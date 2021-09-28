# il2cpp
c++ il2cpp wrapper (for rust, adapt for other games)

for full usage need crc (https://gist.github.com/iwanders/8e1cb7b92af2ccf8d1a73450d771f483)

example usage ~~

inside of a class: FIELD("Assembly-CSharp::Wearable::renderers", renderers, List<Renderer_*>*);
usage: class_instance->renderers();

example offset usage ~~
inside of a method:
static auto off = OFFSET("Assembly-CSharp::ItemDefinition::shortname");
var "off" is now usable as an offset

example class usage ~~
inside of a method:
static auto clazz = CLASS("Assembly-CSharp::PlayerEyes");
you can now do, for example, clazz->static_fields to get address of static fields and proceed

figure out the rest yourself

# note
on entry point (before usage) use init_methods(), init_classes() and init_fields().
