#include "shared/UMultFamily.hpp"

using UMult14Base = UMultModule<14>;

struct UMult14Panel : SubPanel::LabeledPanel {
    UMult14Panel() : SubPanel::LabeledPanel(4, "U·Mult·14") {}
};

struct UMult14Widget : UMultCompactWidget<14, UMult14Base, UMult14Panel> {
    UMult14Widget(UMult14Base* module) : UMultCompactWidget<14, UMult14Base, UMult14Panel>(module, 4) {}
};

Model* modelUMult14 = createModel<UMult14Base, UMult14Widget>("U-Mult-14");
