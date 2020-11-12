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

	addChild( new IntPlug( "axis", Plug::In, BendAxis::YAxis, BendAxis::XAxis, BendAxis::ZAxis ) );
	addChild( new FloatPlug( "angle" ) );
	addChild( new FloatPlug( "upperBound" ) );
	addChild( new FloatPlug( "lowerBound" ) );
}

BendDeformer::~BendDeformer()
{
}

IntPlug* BendDeformer::axisPlug()
{
	return getChild<IntPlug>( g_firstPlugIndex );
}

const IntPlug* BendDeformer::axisPlug( ) const
{
	return getChild<IntPlug>( g_firstPlugIndex );
}

FloatPlug* BendDeformer::anglePlug( )
{
	return getChild<FloatPlug>( g_firstPlugIndex + 1 );
}
const FloatPlug* BendDeformer::anglePlug( ) const
{
	return getChild<FloatPlug>( g_firstPlugIndex + 1 );
}

FloatPlug* BendDeformer::upperBoundPlug( )
{
	return getChild<FloatPlug>( g_firstPlugIndex + 2 );
}
const FloatPlug* BendDeformer::upperBoundPlug( ) const
{
	return getChild<FloatPlug>( g_firstPlugIndex + 2 );
}

FloatPlug* BendDeformer::lowerBoundPlug( )
{
	return getChild<FloatPlug>( g_firstPlugIndex + 3 );
}
const FloatPlug* BendDeformer::lowerBoundPlug( ) const
{
	return getChild<FloatPlug>( g_firstPlugIndex + 3 );
}

void BendDeformer::affects( const Plug *input, AffectedPlugsContainer &outputs ) const
{
	Deformer::affects( input, outputs );

	// TODO: check what has to be done here
	// NOTE: probably nothing because there is no second ScenePlug nor a StringPlug
	// that might be a path

//	if( input->parent<ScenePlug>() == aPlug() || input == bPlug() )
//	{
//		outputs.push_back( outPlug()->boundPlug() );
//		outputs.push_back( outPlug()->objectPlug() );
//	}
}

bool BendDeformer::affectsProcessedObject( const Gaffer::Plug *input ) const
{
	return true;
}

void BendDeformer::hashProcessedObject( const ScenePath &path, const Context *context, IECore::MurmurHash &h ) const
{
	h.append( axisPlug()->hash() );
	h.append( anglePlug()->hash() );
	h.append( upperBoundPlug()->hash() );
	h.append( lowerBoundPlug()->hash() );
}

IECore::ConstObjectPtr BendDeformer::computeProcessedObject( const ScenePath &path, const Gaffer::Context *context, const IECore::Object *inputObject ) const
{
	IECore::ObjectPtr outputObject = inputObject->copy();

	// TODO: modify the object here

	return outputObject;
}
