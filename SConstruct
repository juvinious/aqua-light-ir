import os

env = Environment(ENV = os.environ)
VariantDir('build', '.')

env.ParseConfig('pkg-config --libs allegro-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5')
env.Program('ali-emu', ['build/ali-emu.cpp', 'build/common/tools.cpp'])
