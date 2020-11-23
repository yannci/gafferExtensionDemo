#include "DemoGafferExtension/BendDeformer.h"

#include "IECoreScene/Primitive.h"
#include "IECoreScene/PrimitiveVariable.h"

#include "IECore/VectorTypedData.h"
#include "IECore/AngleConversion.h"

#include "OpenEXR/ImathVec.h"
#include "OpenEXR/ImathFun.h"

IE_CORE_DEFINERUNTIMETYPED( DemoGafferExtension::BendDeformer );

using namespace Gaffer;
using namespace GafferScene;
using namespace DemoGafferExtension;

size_t BendDeformer::g_firstPlugIndex = 0;

BendDeformer::BendDeformer( const std::string &name )
	:	Deformer( name )
{
	storeIndexOfNextChild( g_firstPlugIndex );

	addChild( new IntPlug( "bendAxis", Plug::In, BendAxis::YAxis, BendAxis::XAxis, BendAxis::ZAxis ) );
	addChild( new FloatPlug( "angle" ) );
	addChild( new FloatPlug( "bendPoint" ) );
	addChild( new IntPlug( "captureAxis", Plug::In, BendAxis::XAxis, BendAxis::XAxis, BendAxis::ZAxis ) );
	addChild( new FloatPlug( "upperBound" , Plug::In, 1.0f ) );
	addChild( new FloatPlug( "lowerBound" ) );

	using namespace std::placeholders;
	/// Mapping of the various axis to their correct argument order for the computeBend and createPoint methods
	axisMapping[BendAxis::XAxis] = std::make_pair( std::bind( &BendDeformer::computeBend, this, _1, _2, _3, _4),
												   std::bind( &BendDeformer::createPoint, this, _1, _2, _3 ) );
	axisMapping[BendAxis::YAxis] = std::make_pair( std::bind( &BendDeformer::computeBend, this, _2, _1, _3, _4),
												   std::bind( &BendDeformer::createPoint, this, _2, _1, _3 ) );
	axisMapping[BendAxis::ZAxis] = std::make_pair( std::bind( &BendDeformer::computeBend, this, _3, _2, _1, _4),
												   std::bind( &BendDeformer::createPoint, this, _3, _2, _1 ) );
}

BendDeformer::~BendDeformer()
{
}

IntPlug* BendDeformer::bendAxisPlug()
{
	return getChild<IntPlug>( g_firstPlugIndex );
}

const IntPlug* BendDeformer::bendAxisPlug() const
{
	return getChild<IntPlug>( g_firstPlugIndex );
}

FloatPlug* BendDeformer::anglePlug()
{
	return getChild<FloatPlug>( g_firstPlugIndex + 1 );
}

const FloatPlug* BendDeformer::anglePlug() const
{
	return getChild<FloatPlug>( g_firstPlugIndex + 1 );
}

FloatPlug* BendDeformer::bendPointPlug()
{
	return getChild<FloatPlug>( g_firstPlugIndex + 2 );
}

const FloatPlug* BendDeformer::bendPointPlug() const
{
	return getChild<FloatPlug>( g_firstPlugIndex + 2 );
}

IntPlug* BendDeformer::captureAxisPlug()
{
	return getChild<IntPlug>( g_firstPlugIndex + 3 );
}

const IntPlug* BendDeformer::captureAxisPlug() const
{
	return getChild<IntPlug>( g_firstPlugIndex + 3 );
}

FloatPlug* BendDeformer::upperBoundPlug()
{
	return getChild<FloatPlug>( g_firstPlugIndex + 4 );
}

const FloatPlug* BendDeformer::upperBoundPlug() const
{
	return getChild<FloatPlug>( g_firstPlugIndex + 4 );
}

FloatPlug* BendDeformer::lowerBoundPlug()
{
	return getChild<FloatPlug>( g_firstPlugIndex + 5 );
}

const FloatPlug* BendDeformer::lowerBoundPlug() const
{
	return getChild<FloatPlug>( g_firstPlugIndex + 5 );
}

bool BendDeformer::affectsProcessedObject( const Gaffer::Plug *input ) const
{
	return Deformer::affectsProcessedObject( input ) ||
	input == bendAxisPlug() ||
	input == anglePlug() ||
	input == bendPointPlug() ||
	input == captureAxisPlug() ||
	input == upperBoundPlug() ||
	input == lowerBoundPlug()
	;
}

void BendDeformer::hashProcessedObject( const ScenePath &path, const Context *context, IECore::MurmurHash &h ) const
{
	Deformer::hashProcessedObject( path, context, h );
	h.append( bendAxisPlug()->hash() );
	h.append( anglePlug()->hash() );
	h.append( bendPointPlug()->hash() );
	h.append( captureAxisPlug()->hash() );
	h.append( upperBoundPlug()->hash() );
	h.append( lowerBoundPlug()->hash() );
}

IECore::ConstObjectPtr BendDeformer::computeProcessedObject( const ScenePath &path, const Gaffer::Context *context, const IECore::Object *inputObject ) const
{
	if ( 0 == anglePlug()->getValue() )
	{
		return inputObject;
	}

	/// Use the primitive base class to make the deformer work for mesh and curves primitives.
	/// This might have side effects I don't know of but none were noticed during testing
	const IECoreScene::Primitive *prim = IECore::runTimeCast<const IECoreScene::Primitive>( inputObject );
	if( !prim )
	{
		return inputObject;
	}

	const IECore::V3fVectorData *positionData = prim->variableData<IECore::V3fVectorData>( "P" , IECoreScene::PrimitiveVariable::Vertex );
	const IECore::V3fVectorData::ValueType &points = positionData->readable();

	IECore::V3fVectorData* bendedPoints = new IECore::V3fVectorData();

	std::vector<float> axisComponent{ 0 };
	axisComponent.reserve( points.size() );
	axisComponent.resize( points.size() );

	/// Compute the `length` of the capture axis values
	auto captureAxisValue = captureAxisPlug()->getValue();
	std::transform( points.begin(), points.end(), axisComponent.begin(), [&captureAxisValue](Imath::V3f point) { return point[captureAxisValue]; } );

	float minAxisP = *std::min_element( axisComponent.begin(), axisComponent.end() );
	float maxAxisP = *std::max_element( axisComponent.begin(), axisComponent.end() );
	float length = maxAxisP - minAxisP;

	if ( 0 == length )
	{
		return inputObject;
	}

	/// Get the mapped std::function objects for the chosen bending axis. I do this to use one
	/// computeBend method that works for all coordinates instead of using switches or ifs
	auto bendAxisValue = bendAxisPlug()->getValue();
	auto computeBendFn = axisMapping.at( static_cast<BendAxis>( bendAxisValue ) ).first;

	std::vector<Imath::V3f> &pointBendVec = bendedPoints->writable();
	pointBendVec.reserve( points.size() );

	for (auto point : points) {
		pointBendVec.push_back( computeBendFn( point[2], point[1], point[0], length ) );
	}

	IECoreScene::PrimitivePtr result = prim->copy();
	IECoreScene::PrimitiveVariable newP = IECoreScene::PrimitiveVariable( IECoreScene::PrimitiveVariable::Vertex, bendedPoints );

	result->variables["P"] = newP;

	return result;
}

Imath::V3f BendDeformer::createPoint( float x, float y, float z ) const
{
	return Imath::V3f(x, y, z);
}

Imath::V3f BendDeformer::computeBend( float staticP, float axisP, float perpP, float length ) const
{
	/// Divide the angle by the `length` of the capture axis. The publication of A. Barr does not define
	/// the global or deform space but it only works correct if values lie between 0 and 1. That would make
	/// sense because of the usage of sinus and cosinus for the coordinate computation.
	float angle = anglePlug()->getValue() / length ;
	float k = IECore::degreesToRadians(angle);
	float curvatureRadius = 1.0f / k;

	float lowerBound = lowerBoundPlug()->getValue();
	float upperBound = upperBoundPlug()->getValue();
	float y_0 = bendPointPlug()->getValue();

	float y_hat = axisP;
	if ( axisP <= lowerBound )
	{
		y_hat = lowerBound;
	}
	if ( axisP >= upperBound )
	{
		y_hat = upperBound;
	}

	float theta = k * (y_hat - y_0);
	float Ctheta = Imath::Math<float>::cos( theta );
	float Stheta = Imath::Math<float>::sin( theta );

	float newAxisP = -Stheta * ( perpP - curvatureRadius ) + y_0;
	float newPerpP = Ctheta * ( perpP - curvatureRadius ) + curvatureRadius;

	if ( axisP < lowerBound )
	{
		newAxisP += Ctheta * (axisP - lowerBound);
		newPerpP += Stheta * (axisP - lowerBound);
	}
	if ( axisP > upperBound )
	{
		newAxisP += Ctheta * (axisP - upperBound);
		newPerpP += Stheta * (axisP - upperBound);
	}

	auto bendAxisValue = static_cast<BendAxis>( bendAxisPlug()->getValue() );
	auto createPointFn = axisMapping.at( bendAxisValue ).second;
	return createPointFn( staticP, newAxisP, newPerpP );
}
