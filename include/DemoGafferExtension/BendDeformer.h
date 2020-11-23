#ifndef DEMOGAFFEREXTENSION_BENDDEFORMER_H
#define DEMOGAFFEREXTENSION_BENDDEFORMER_H

#include "DemoGafferExtension/TypeIds.h"

#include "Gaffer/NumericPlug.h"

#include "GafferScene/Deformer.h"
#include "GafferScene/ScenePlug.h"

#include "OpenEXR/ImathVec.h"

#include <functional>
#include <map>
#include <utility>

namespace DemoGafferExtension {

class BendDeformer : public GafferScene::Deformer {

	public:

		BendDeformer( const std::string &name = defaultName<BendDeformer>() );
		~BendDeformer() override;

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( DemoGafferExtension::BendDeformer, TypeId::BendDeformerTypeId, GafferScene::Deformer );

		enum BendAxis
		{
			XAxis = 0,
			YAxis = 1,
			ZAxis = 2
		};

		Gaffer::IntPlug *bendAxisPlug();
		const Gaffer::IntPlug *bendAxisPlug() const;

		Gaffer::FloatPlug *anglePlug();
		const Gaffer::FloatPlug *anglePlug() const;

		Gaffer::FloatPlug *bendPointPlug();
		const Gaffer::FloatPlug *bendPointPlug() const;

		Gaffer::IntPlug *captureAxisPlug();
		const Gaffer::IntPlug *captureAxisPlug() const;

		Gaffer::FloatPlug *upperBoundPlug();
		const Gaffer::FloatPlug *upperBoundPlug() const;

		Gaffer::FloatPlug *lowerBoundPlug();
		const Gaffer::FloatPlug *lowerBoundPlug() const;

		bool affectsProcessedObject(const Gaffer::Plug *input) const override;
		void hashProcessedObject(const ScenePath &path, const Gaffer::Context *context, IECore::MurmurHash &h) const override;
		IECore::ConstObjectPtr computeProcessedObject(const ScenePath &path, const Gaffer::Context *context, const IECore::Object *inputObject) const override;

	private:

		static size_t g_firstPlugIndex;
		/// Simple factory method that returns a Vector constructed from the passed values.
		/// We need this factory method to bind the passed values differently to the arguments
		/// since this is not possible with a constructor
		Imath::V3f createPoint( float x, float y, float z ) const;
		/// Computes the bended coordinates. We use this helper method instead of a direct implementation in
		/// computeProcessedObject to bind the various coordinate values differently depending on the bend axis.
		Imath::V3f computeBend( float staticP, float axisP, float perpP, float length ) const;

		/// Used to map the different function bindings to the respective axis
		std::map<BendAxis, std::pair< std::function<Imath::V3f(float, float, float, float)>,
									  std::function<Imath::V3f(float, float, float)>>> axisMapping;
};

} // namespace DemoGafferExtension

#endif // DEMOGAFFEREXTENSION_BENDDEFORMER_H
