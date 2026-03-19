#include "shared/UMultFamily.hpp"

using UMult6Base = UMultModule<6>;

struct UMult6Panel : SubPanel::LabeledPanel {
    UMult6Panel() : SubPanel::LabeledPanel(4, "U·Mult·6") {}
};

struct UMult6Widget : UMultCompactWidget<6, UMult6Base, UMult6Panel> {
    UMult6Widget(UMult6Base* module) : UMultCompactWidget<6, UMult6Base, UMult6Panel>(module, 4) {}
};

Model* modelUMult6 = createModel<UMult6Base, UMult6Widget>("U-Mult-6");
