#include "shared/UMultFamily.hpp"

using UMult8Base = UMultModule<8>;

struct UMult8Panel : SubPanel::LabeledPanel {
    UMult8Panel() : SubPanel::LabeledPanel(4, "U·Mult·8") {}
};

struct UMult8Widget : UMultCompactWidget<8, UMult8Base, UMult8Panel> {
    UMult8Widget(UMult8Base* module) : UMultCompactWidget<8, UMult8Base, UMult8Panel>(module, 4) {}
};

Model* modelUMult8 = createModel<UMult8Base, UMult8Widget>("U-Mult-8");
