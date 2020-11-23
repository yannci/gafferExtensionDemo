import Gaffer
import GafferUI

import DemoGafferExtension


Gaffer.Metadata.registerNode(

	DemoGafferExtension.BendDeformer,

	"description",
	"""
	Bend the input geometry around the given axis.
	""",

	plugs = {

		"bendAxis" : [

			"description",
			"""
			The axis to bend around.
			""",

			"preset:X Axis", 0,
			"preset:Y Axis", 1,
			"preset:Z Axis", 2,

			"plugValueWidget:type", "GafferUI.PresetsPlugValueWidget",

		],

		"angle" : [

			"description",
			"""
			The angle of the bend.
			""",

		],

		"bendPoint" : [

			"description",
			"""
			The offset along the bend axis where the bend happens.
			""",

		],

		"captureAxis" : [

			"description",
			"""
			The axis along the points to bend are captured.
			""",

			"preset:X Axis", 0,
			"preset:Y Axis", 1,
			"preset:Z Axis", 2,

			"plugValueWidget:type", "GafferUI.PresetsPlugValueWidget",

		],

		"upperBound" : [

			"description",
			"""
			The maximum point position that is influenced by the bend.
			""",

		],

		"lowerBound" : [

			"description",
			"""
			The minimum point position that is influenced by the bend.
			""",

		],
	}

)
