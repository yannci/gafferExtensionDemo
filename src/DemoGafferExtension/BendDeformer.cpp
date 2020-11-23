#include "DemoGafferExtension/BendDeformer.h"

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
	return inputObject;
}
