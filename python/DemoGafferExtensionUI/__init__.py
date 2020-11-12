__import__( "Gaffer" )
__import__( "GafferUI" )
__import__( "GafferScene" )
__import__( "GafferSceneUI" )

import DemoSceneProcessorUI
import BendDeformerUI

__import__( "IECore" ).loadConfig( "GAFFER_STARTUP_PATHS", subdirectory = "DemoGafferExtensionUI" )

