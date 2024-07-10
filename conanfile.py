import os.path
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.files import copy


class NNToolKitRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    #generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("fmt/10.2.1")
        self.requires("spdlog/1.14.1")
        self.requires("flatbuffers/24.3.25")
        self.requires("qt/6.4.2")
    
    def layout(self):
        ## define project folder structure
        #self.folders.generators = "generators"
        cmake_layout(self)

    def validate(self):
        if self.settings.os == "Macos" and self.settings.arch == "armv8":
            raise ConanInvalidConfiguration("ARM v8 not supported in Macos")
    
    def configure(self):
        self.options["spdlog"].header_only = False

        self.options["qt"].shared = True
        self.options["qt"].commercial = False
        self.options["qt"].openssl = False
        self.options["qt"].with_pcre2 = True
        self.options["qt"].with_glib = False
        self.options["qt"].with_doubleconversion = True
        self.options["qt"].with_harfbuzz = False
        self.options["qt"].with_libjpeg = False
        self.options["qt"].with_libpng = False
        self.options["qt"].with_sqlite3 = False
        self.options["qt"].with_mysql = False
        self.options["qt"].with_pq = False
        self.options["qt"].with_odbc = False
        self.options["qt"].with_openal = False
        self.options["qt"].with_zstd = False
        self.options["qt"].with_gstreamer = False
        self.options["qt"].with_pulseaudio = False
        self.options["qt"].with_dbus = False
        self.options["qt"].with_atspi = False
        self.options["qt"].with_md4c = False
        self.options["qt"].gui = True
        self.options["qt"].widgets = True
        self.options["qt"].device = False
        self.options["qt"].cross_compile = False
        self.options["qt"].sysroot = False
        self.options["qt"].config = False
        self.options["qt"].multiconfiguration = False
        self.options["qt"].qtsvg = False
        self.options["qt"].qtdeclarative = False
        self.options["qt"].qtactiveqt = False
        self.options["qt"].qtscript = False
        self.options["qt"].qtmultimedia = False
        self.options["qt"].qttools = False
        self.options["qt"].qtxmlpatterns = False
        self.options["qt"].qttranslations = False
        self.options["qt"].qtdoc = False
        self.options["qt"].qtlocation = False
        self.options["qt"].qtsensors = False
        self.options["qt"].qtconnectivity = False
        self.options["qt"].qtwayland = False
        self.options["qt"].qt3d = False
        self.options["qt"].qtimageformats = False
        self.options["qt"].qtgraphicaleffects = False
        self.options["qt"].qtquickcontrols = False
        self.options["qt"].qtserialbus = False
        self.options["qt"].qtserialport = False
        self.options["qt"].qtwinextras = False
        self.options["qt"].qtandroidextras = False
        self.options["qt"].qtwebsockets = False
        self.options["qt"].qtwebchannel = False
        self.options["qt"].qtwebengine = False
        self.options["qt"].qtwebview = False
        self.options["qt"].qtquickcontrols2 = False
        self.options["qt"].qtpurchasing = False
        self.options["qt"].qtcharts = False
        self.options["qt"].qtdatavis3d = False
        self.options["qt"].qtvirtualkeyboard = False
        self.options["qt"].qtgamepad = False
        self.options["qt"].qtscxml = False
        self.options["qt"].qtspeech = False
        self.options["qt"].qtnetworkauth = False
        self.options["qt"].qtremoteobjects = False
        self.options["qt"].qtwebglplugin = False
        self.options["qt"].qtlottie = False
        self.options["qt"].qtquicktimeline = False
        self.options["qt"].qtquick3d = False
        self.options["qt"].qtknx = False
        self.options["qt"].qtmqtt = False
        self.options["qt"].qtcoap = False
        self.options["qt"].qtopcua = False
        self.options["qt"].essential_modules = False
        self.options["qt"].addon_modules = False
        self.options["qt"].deprecated_modules = False
        self.options["qt"].preview_modules = False

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        tc = CMakeToolchain(self, generator="Ninja")
        tc.generate()

        conan_bin_dir = self.build_folder + "/bin"
        for dep in self.dependencies.values():
            print("{}".format(dep.cpp_info.bindirs[0]))
            copy(self, "*.dll", dep.cpp_info.bindirs[0], conan_bin_dir)
            copy(self, "*.exe", dep.cpp_info.bindirs[0], conan_bin_dir)
            copy(self, "*.lib", dep.cpp_info.libdirs[0], conan_bin_dir)
        