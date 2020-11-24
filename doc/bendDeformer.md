# Bend deformer #

The bend deformer allows to bend an object in a specified angle around the various axis
in the local space of the object.

To give more control to the artist, the lower and upper bound of the bend range (along the axis) can be restricted and the bend point can be offsetted.

The implementation of the deformer is based on the publication of Alan Barr. For
more in depth knowledge of the algorithm check:

Alan H. Barr. 1984. Global and local deformations of solid primitives.
SIGGRAPH Comput. Graph. 18, 3 (July 1984), 21–30.
DOI:https://doi.org/10.1145/964965.808573

## Parameters ##

To control how the object is bended we provide multiple parameters that influence the behavior of the
deformer.

#### Bend Axis ####
The bend axis defines the axis the object is bend around.

#### Angle ####
The angle of the bend.

#### Bend Point ####
The bend point is an offset along the bend axis that defines the origin of the bend.

#### Capture Axis ####
The capture axis defines the axis of the object we calculate various parameters of. Taking a line
pointing in positive Y direction based at the origin the capture axis would be the Y axis.

#### Upper Bound ####
The upper bound is the maximum value along the bend axis we consider for bending.

Taking a line pointing in positive Y direction based at the origin with a length of 1, setting this parameter
to 0.5 would result in points that have a Y coordinate greater than this value not being bend (assuming the
capture axis is set to Y)

#### Lower Bound ####
The lower bound is the minimum value along the bend axis we consider for bending.

Taking a line pointing in positive Y direction based at the origin with a length of 1, setting this parameter
to 0.5 would result in points that have a Y coordinate less than this value not being bend (assuming the
capture axis is set to Y)

## Use cases and examples ##
Possible use cases for the bend deformer are classical animation where one could use the deformer to bend the finger tip of a character. Furthermore it could be used to create guide hair on a character that might get replicated in a shader at render time or to create a simple wind animation of grass.

To get a better understanding of what and how things can be done with the bend deformer see the `bendingGrass.gfr` scene in the examples directory where an exemplary scene of the aforementioned wind animation of grass is shown.

