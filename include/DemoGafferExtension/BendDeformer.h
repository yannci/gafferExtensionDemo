#ifndef DEMOGAFFEREXTENSION_BENDDEFORMER_H
#define DEMOGAFFEREXTENSION_BENDDEFORMER_H

#include "DemoGafferExtension/TypeIds.h"

#include "GafferScene/Deformer.h"
#include "GafferScene/ScenePlug.h"


#include "Gaffer/NumericPlug.h"

namespace DemoGafferExtension
{

class BendDeformer : public GafferScene::Deformer
{

	public:


		BendDeformer(const std::string &name = defaultName<BendDeformer>());
		~BendDeformer() override;

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION(DemoGafferExtension::BendDeformer, TypeId::BendDeformerTypeId, GafferScene::Deformer);

		enum BendAxis
		{
			XAxis = 0,
			YAxis = 1,
			ZAxis = 2
		};

		Gaffer::IntPlug *axisPlug();
		const Gaffer::IntPlug *axisPlug() const;

		Gaffer::FloatPlug *anglePlug();
		const Gaffer::FloatPlug *anglePlug() const;

		Gaffer::FloatPlug *upperBoundPlug();
		const Gaffer::FloatPlug *upperBoundPlug() const;

		Gaffer::FloatPlug *lowerBoundPlug();
		const Gaffer::FloatPlug *lowerBoundPlug() const;

		// TODO: check if bounds have to be processed or if the Deformer Baseclass implementation does this
		void affects(const Gaffer::Plug *input, AffectedPlugsContainer &outputs) const override;
		bool affectsProcessedObject( const Gaffer::Plug *input ) const override;
		void hashProcessedObject( const ScenePath &path, const Gaffer::Context *context, IECore::MurmurHash &h ) const override;
		IECore::ConstObjectPtr computeProcessedObject( const ScenePath &path, const Gaffer::Context *context, const IECore::Object *inputObject ) const override;

	private :
		static size_t g_firstPlugIndex;

};

} // namespace DemoGafferExtension

#endif // DEMOGAFFEREXTENSION_BENDDEFORMER_H
