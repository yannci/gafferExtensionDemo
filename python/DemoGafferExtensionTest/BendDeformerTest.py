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

	def testLineEndPoints( self ) :
		""" Test different point numbers of a line """

		# test line with 50 points
		# ========================

		line = self.makeLine()

		bendNode = DemoGafferExtension.BendDeformer()
		bendNode["in"].setInput( line["out"] )

		pathFilter = GafferScene.PathFilter( "PathFilter" )
		pathFilter["paths"].setValue( IECore.StringVectorData( [ '/object' ] ) )
		bendNode["filter"].setInput( pathFilter["out"] )

		bendNode["bendAxis"].setValue( 2 )
		bendNode["captureAxis"].setValue( 1 )
		bendNode["angle"].setValue( 360 )
		bendNode["bendPoint"].setValue( 0.0 )
		bendNode["upperBound"].setValue( 2*pi )

		bendedLine = bendNode["out"].object( "/object" )
		self.assertEqual( len(bendedLine["P"].data), 50	)

		firstPoint = bendedLine["P"].data[0]
		lastPoint = bendedLine["P"].data[49]

		firstPointX = firstPoint.x
		lastPointX = lastPoint.x

		firstPointY = firstPoint.y
		lastPointY = lastPoint.y

		firstPointZ = firstPoint.z
		lastPointZ = lastPoint.z

		self.assertAlmostEqual( firstPoint.x, lastPoint.x, delta=0.5 )
		self.assertAlmostEqual( firstPoint.y, lastPoint.y, delta=0.5 )
		self.assertAlmostEqual( firstPoint.z, lastPoint.z, delta=0.5 )

		del line, bendNode, pathFilter, bendedLine, firstPoint, lastPoint

		# test line with 1000 points
		# ==========================

		line = self.makeLine( numPt=1000 )

		bendNode = DemoGafferExtension.BendDeformer()
		bendNode["in"].setInput( line["out"] )

		pathFilter = GafferScene.PathFilter( "PathFilter" )
		pathFilter["paths"].setValue( IECore.StringVectorData( [ '/object' ] ) )
		bendNode["filter"].setInput( pathFilter["out"] )

		bendNode["bendAxis"].setValue( 2 )
		bendNode["captureAxis"].setValue( 1 )
		bendNode["angle"].setValue( 360 )
		bendNode["bendPoint"].setValue( 0.0 )
		bendNode["upperBound"].setValue( 2*pi )

		bendedLine = bendNode["out"].object( "/object" )
		self.assertEqual( len(bendedLine["P"].data), 1000 )

		firstPoint = bendedLine["P"].data[0]
		lastPoint = bendedLine["P"].data[49]

		firstPointX = firstPoint.x
		lastPointX = lastPoint.x

		firstPointY = firstPoint.y
		lastPointY = lastPoint.y

		firstPointZ = firstPoint.z
		lastPointZ = lastPoint.z

		self.assertAlmostEqual( firstPoint.x, lastPoint.x, delta=0.5 )
		self.assertAlmostEqual( firstPoint.y, lastPoint.y, delta=0.5 )
		self.assertAlmostEqual( firstPoint.z, lastPoint.z, delta=0.5 )

		del line, bendNode, pathFilter, bendedLine, firstPoint, lastPoint

		# test line with 100'000 points
		# =============================

		line = self.makeLine( numPt=100000 )

		bendNode = DemoGafferExtension.BendDeformer()
		bendNode["in"].setInput( line["out"] )

		pathFilter = GafferScene.PathFilter( "PathFilter" )
		pathFilter["paths"].setValue( IECore.StringVectorData( [ '/object' ] ) )
		bendNode["filter"].setInput( pathFilter["out"] )

		bendNode["bendAxis"].setValue( 2 )
		bendNode["captureAxis"].setValue( 1 )
		bendNode["angle"].setValue( 360 )
		bendNode["bendPoint"].setValue( 0.0 )
		bendNode["upperBound"].setValue( 2*pi )

		bendedLine = bendNode["out"].object( "/object" )
		self.assertEqual( len(bendedLine["P"].data), 100000 )

		firstPoint = bendedLine["P"].data[0]
		lastPoint = bendedLine["P"].data[49]

		firstPointX = firstPoint.x
		lastPointX = lastPoint.x

		firstPointY = firstPoint.y
		lastPointY = lastPoint.y

		firstPointZ = firstPoint.z
		lastPointZ = lastPoint.z

		self.assertAlmostEqual( firstPoint.x, lastPoint.x, delta=0.5 )
		self.assertAlmostEqual( firstPoint.y, lastPoint.y, delta=0.5 )
		self.assertAlmostEqual( firstPoint.z, lastPoint.z, delta=0.5 )

	def testLineFivePointsCoordinates( self ) :
		""" Test the bend of a line with 5 points around the Z axis.
		A 360 degree bend is supposed to result in a circle or in the case of 5
		points in a square. Due to the line length of 2*pi the resulting
		coordinates are known from the unit circle """

		line5pt = self.makeLine( numPt=5 )

		bendNode = DemoGafferExtension.BendDeformer()
		bendNode["in"].setInput( line5pt["out"] )

		pathFilter = GafferScene.PathFilter( "PathFilter" )
		pathFilter["paths"].setValue( IECore.StringVectorData( [ '/object' ] ) )
		bendNode["filter"].setInput( pathFilter["out"] )

		bendNode["bendAxis"].setValue( 2 )
		bendNode["captureAxis"].setValue( 1 )
		bendNode["angle"].setValue( 360 )
		bendNode["bendPoint"].setValue( 0.0 )
		bendNode["upperBound"].setValue( 2*pi )

		bendedLine = bendNode["out"].object( "/object" )
		self.assertEqual( len(bendedLine["P"].data), 5 )

		firstPoint = bendedLine["P"].data[0]
		secondPoint = bendedLine["P"].data[1]
		thirdPoint = bendedLine["P"].data[2]
		fourthPoint = bendedLine["P"].data[3]
		fifthPoint = bendedLine["P"].data[4]

		self.assertAlmostEqual( abs(firstPoint.y), abs(thirdPoint.y), delta=0.05 )
		self.assertAlmostEqual( abs(secondPoint.x), abs(fourthPoint.x), delta=0.05 )

		self.assertEqual( firstPoint.x, 0.0 )
		self.assertAlmostEqual( secondPoint.x, 1.0, delta=0.05 )
		self.assertAlmostEqual( thirdPoint.x, 2.0, delta=0.05 )
		self.assertAlmostEqual( fourthPoint.x, 1.0, delta=0.05 )
		self.assertAlmostEqual( fifthPoint.x, 0.0, delta=0.05 )

		self.assertAlmostEqual( abs(secondPoint.y - fourthPoint.y), 2, delta=0.05 )
		self.assertAlmostEqual( abs(firstPoint.x - thirdPoint.x), 2, delta=0.05 )

	def testLineBendAxisSwitch( self ):
		""" Test switching the bending axis """
		line50pt = self.makeLine()

		bendNode = DemoGafferExtension.BendDeformer()
		bendNode["in"].setInput( line50pt["out"] )

		pathFilter = GafferScene.PathFilter( "PathFilter" )
		pathFilter["paths"].setValue( IECore.StringVectorData( [ '/object' ] ) )
		bendNode["filter"].setInput( pathFilter["out"] )

		bendNode["angle"].setValue( 360 )
		bendNode["bendPoint"].setValue( 0.0 )
		bendNode["captureAxis"].setValue( 1 )
		bendNode["upperBound"].setValue( 2*pi )

		bendedLine = bendNode["out"].object( "/object" )
		self.assertEqual( len(bendedLine["P"].data), 50 )

		# bend around x axis
		# ==================
		bendNode["bendAxis"].setValue( 0 )
		for P in bendedLine["P"].data:
			self.assertEqual( P.x, 0 )
			self.assertLess( P.y, 2*pi+0.1 )
			self.assertGreaterEqual( P.z, 0 )

		# bend around y axis
		# ==================
		bendNode["bendAxis"].setValue( 1 )
		for P in bendedLine["P"].data:
			self.assertEqual( P.x, 0 )
			self.assertLessEqual( P.y, 2*pi+0.1 )
			self.assertGreaterEqual( P.y, 0 )
			self.assertEqual( P.z, 0 )

		# bend around z axis
		# ==================
		bendNode["bendAxis"].setValue( 2 )
		for P in bendedLine["P"].data:
			self.assertGreaterEqual( P.x, 0 )
			self.assertEqual( P.z, 0 )
			self.assertLess( P.y, 2*pi+0.1 )

if __name__ == "__main__":
	unittest.main()
