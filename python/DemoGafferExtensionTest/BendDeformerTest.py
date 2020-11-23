from math import pi
import unittest

import imath

import IECore
import IECoreScene

import Gaffer
import GafferScene

import GafferTest
import GafferSceneTest

import DemoGafferExtension

class BendDeformerTest( GafferSceneTest.SceneTestCase ) :

	def makeLine( self, numPt=50, length=2*pi ):
		""" Create a objectToScene node that holds a line primitive.
		By default the line has 50 points, starts at the origin (0, 0, 0) pointing in positive Y direction.
		The length is 2 * pi, the circumference of the unit circle """

		line = IECore.LineSegment3f( imath.V3f( 0, 0, 0 ), imath.V3f( 0, length, 0 ) )
		points = [ line( 0 + (x-1) * 1.0/(numPt-1) ) for x in range( 1, numPt+1 ) ]

		curve = IECoreScene.CurvesPrimitive(

				IECore.IntVectorData( [ numPt ] ),
				IECore.CubicBasisf.linear(),
				False,
				IECore.V3fVectorData( points )

				)

		objectToScene = GafferScene.ObjectToScene()
		objectToScene["object"].setValue( curve )

		return objectToScene

	def testConstruct( self ) :

		bendNode = DemoGafferExtension.BendDeformer()
		self.assertEqual( bendNode.getName(), "BendDeformer" )

	def testPlugs( self ):

		bendNode = DemoGafferExtension.BendDeformer()

		bendAxisPlug = bendNode["bendAxis"]
		self.assertIsNotNone( bendAxisPlug )
		self.assertIsInstance( bendAxisPlug, Gaffer.IntPlug )

		anglePlug = bendNode["angle"]
		self.assertIsNotNone( anglePlug )
		self.assertIsInstance( anglePlug, Gaffer.FloatPlug )

		bendPointPlug = bendNode["bendPoint"]
		self.assertIsNotNone( bendPointPlug )
		self.assertIsInstance( bendPointPlug, Gaffer.FloatPlug )

		captureAxisPlug = bendNode["captureAxis"]
		self.assertIsNotNone( captureAxisPlug )
		self.assertIsInstance( captureAxisPlug, Gaffer.IntPlug )

		upperBoundPlug = bendNode["upperBound"]
		self.assertIsNotNone( upperBoundPlug )
		self.assertIsInstance( upperBoundPlug, Gaffer.FloatPlug )

		lowerBoundPlug = bendNode["lowerBound"]
		self.assertIsNotNone( lowerBoundPlug )
		self.assertIsInstance( lowerBoundPlug, Gaffer.FloatPlug )


	def testCompute( self ) :
		""" Test that we leave the scene in a valid state after bending a cube """

		bendNode = DemoGafferExtension.BendDeformer()
		cube = GafferScene.Cube()
		bendNode["in"].setInput( cube["out"] )

		bendNode["bendAxis"].setValue( 0 )
		bendNode["angle"].setValue( 90 )
		bendNode["bendPoint"].setValue( 0.5 )
		bendNode["captureAxis"].setValue( 1 )
		bendNode["upperBound"].setValue( 2 )
		bendNode["lowerBound"].setValue( -2 )

		self.assertSceneValid( cube["out"] )

if __name__ == "__main__":
	unittest.main()
