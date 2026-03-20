# Integrazione rapida del pulsante SubSeth

## 1) Aggiungi il file
Copia `SubButton.hpp` in:

```cpp
src/shared/SubButton.hpp
```

## 2) Includilo nel modulo
Esempio:

```cpp
#include "shared/SubButton.hpp"
```

## 3) Configura i parametri nel `Module`
Per un latch:

```cpp
configSwitch(MY_LATCH_PARAM, 0.f, 1.f, 0.f, "My latch");
```

Per un momentary:

```cpp
configButton(MY_MOMENTARY_PARAM, "My momentary");
```

## 4) Aggiungi il widget nel `ModuleWidget`
Uso diretto, senza toccare `SubControls.hpp`:

```cpp
addParam(createParamCentered<SubButton::PurpleLatch>(
	Vec(30.f, 40.f), module, MyModule::MY_LATCH_PARAM
));
```

```cpp
addParam(createParamCentered<SubButton::PurpleMomentary>(
	Vec(30.f, 70.f), module, MyModule::MY_MOMENTARY_PARAM
));
```

## 5) Label opzionale col pannello SubSeth
Se stai usando `SubPanel::LabeledPanel`, puoi etichettarlo così:

```cpp
auto* hold = createParamCentered<SubButton::PurpleLatch>(
	Vec(30.f, 40.f), module, MyModule::HOLD_PARAM
);
addParam(hold);
panel->addObjLabel(hold, "Hold");
```

## 6) Varianti colore già pronte
Latch:
- `SubButton::SilverLatch`
- `SubButton::RedLatch`
- `SubButton::BlueLatch`
- `SubButton::PurpleLatch`
- `SubButton::PinkLatch`

Momentary:
- `SubButton::SilverMomentary`
- `SubButton::RedMomentary`
- `SubButton::BlueMomentary`
- `SubButton::PurpleMomentary`
- `SubButton::PinkMomentary`

## Note
- OFF: cappello neutro
- ON: cappello del colore scelto + halo
- corona esterna nera
- anello intermedio sempre del colore selezionato, matte, anche da spento
- profilo volutamente poco profondo
