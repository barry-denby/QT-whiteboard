# define the marking tool project and state that it is C++ based. here we are stating
# that we also need the compiler to produce position independant code as QT requires it
project('qt_whiteboard', 'cpp')

# import the qt5 module. we need seperate dependancies for the test frame work and also the gui itself
qt5_module = import('qt5')
qt5_components = dependency('qt5', modules: ['Core', 'Gui', 'Widgets'])

# variable for the include directory that will be referenced by subsequent meson.build files
# note that thiqs has to be declared before the subdirs so they will see it
include_dir = [include_directories('/usr/include/qt')]

# the list of source files that will make up the application
source_files = ['main.cpp']

# the marking tool executable that will be produced after building is complete
executable('qt_whiteboard', source_files, include_directories: include_dir, dependencies: qt5_components,  cpp_args: '-fPIC')
