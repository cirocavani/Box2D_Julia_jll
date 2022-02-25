# Note that this script can accept some limited command-line arguments, run
# `julia build_tarballs.jl --help` to see a usage message.
using BinaryBuilder, Pkg

name = "Box2D_Julia"
version = v"2.4.1"

# See https://github.com/JuliaLang/Pkg.jl/issues/2942
# Once this Pkg issue is resolved, this must be removed
uuid = Base.UUID("a83860b7-747b-57cf-bf1f-3e79990d037f")
delete!(Pkg.Types.get_last_stdlibs(v"1.6"), uuid)

# Collection of sources required to complete build
sources = [
    DirectorySource("./src"),
]

# Bash recipe for building across all platforms
script = raw"""
cd $WORKSPACE/srcdir
mkdir -p build
cd build
cmake \
    -DCMAKE_INSTALL_PREFIX=${prefix} \
    -DCMAKE_FIND_ROOT_PATH=${prefix} \
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TARGET_TOOLCHAIN} \
    -DCMAKE_BUILD_TYPE=Release \
    -DJulia_PREFIX=${prefix} \
    ../box2d_julia
cmake \
    --build . \
    --config Release \
    --target install

install_license /usr/share/licenses/MIT
"""

# These are the platforms we will build for by default, unless further
# platforms are passed in on the command line
platforms = supported_platforms()

# The products that we will ensure are always built
products = [
    LibraryProduct("libbox2d_julia", :libbox2d_julia),
]

# Dependencies that must be installed before this package can be built
dependencies = [
    BuildDependency(PackageSpec(;
        name = "libjulia_jll",
        version = v"1.6.0",
    )),
    Dependency("libcxxwrap_julia_jll", v"0.9.0"),
    # Dependency("Box2D_jll", v"2.4.1"),
    Dependency(PackageSpec(;
        name = "Box2D_jll",
        uuid = "6756b63d-2e54-5dc6-816b-1fc83e1ae834",
        path = joinpath(Pkg.devdir(), "Box2D_jll"),
    )),
]

# Build the tarballs, and possibly a `build.jl` as well.
build_tarballs(
    ARGS,
    name,
    version,
    sources,
    script,
    platforms,
    products,
    dependencies;
    julia_compat="1.6",
    preferred_gcc_version = v"7",
)
