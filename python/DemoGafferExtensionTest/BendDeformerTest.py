import unittest

import Gaffer
import GafferScene

import GafferTest
import GafferSceneTest

import DemoGafferExtension

class BendDeformerTest( GafferSceneTest.SceneTestCase ) :

	def testConstruct( self ) :

		node = DemoGafferExtension.BendDeformer()
		self.assertEqual( node.getName(), "BendDeformer" )

	def testPlugs( self ):

		node = DemoGafferExtension.BendDeformer()

		axisPlug = node["axis"]
		self.assertIsNotNone( axisPlug )
		self.assertIsInstance( axisPlug, Gaffer.IntPlug )

		anglePlug = node["angle"]
		self.assertIsNotNone( anglePlug )
		self.assertIsInstance( anglePlug, Gaffer.FloatPlug )

		upperBoundPlug = node["upperBound"]
		self.assertIsNotNone( upperBoundPlug )
		self.assertIsInstance( upperBoundPlug, Gaffer.FloatPlug )

		lowerBoundPlug = node["lowerBound"]
		self.assertIsNotNone( lowerBoundPlug )
		self.assertIsInstance( lowerBoundPlug, Gaffer.FloatPlug )

	@unittest.skip("Not yet implemented")
	def testCompute( self ) :
		pass

	@unittest.skip("Not yet implemented")
	def testAffects( self ) :
		pass

if __name__ == "__main__":
	unittest.main()
