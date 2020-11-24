#include "boost/python.hpp"

#include "DemoGafferExtension/DemoSceneProcessor.h"
#include "DemoGafferExtension/BendDeformer.h"

#include "GafferBindings/DependencyNodeBinding.h"

using namespace boost::python;
using namespace GafferScene;

BOOST_PYTHON_MODULE( _DemoGafferExtension )
{
	GafferBindings::DependencyNodeClass<DemoGafferExtension::DemoSceneProcessor>();
	GafferBindings::DependencyNodeClass<DemoGafferExtension::BendDeformer>();
}
