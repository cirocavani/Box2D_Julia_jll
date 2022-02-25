# Note that this script can accept some limited command-line arguments, run
# `julia build_tarballs.jl --help` to see a usage message.
using BinaryBuilder, Pkg

name = "Box2D_Julia"
version = v"2.4.1"

# https://github.com/JuliaPackaging/Yggdrasil/blob/82eb18cb6222195b9995b98b136f6f5f995fc721/L/libcxxwrap_julia/build_tarballs.jl

# https://github.com/JuliaPackaging/Yggdrasil/blob/24776e4eb2865cc24363cb7ed36850c5f695c820/L/libjulia/build_tarballs.jl
julia_versions = [v"1.6.3", v"1.7.0", v"1.8.0"]

# See https://github.com/JuliaLang/Pkg.jl/issues/2942
# Once this Pkg issue is resolved, this must be removed
uuid = Base.UUID("a83860b7-747b-57cf-bf1f-3e79990d037f")
delete!(Pkg.Types.get_last_stdlibs(v"1.6.3"), uuid)

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
# platforms = supported_platforms()

# https://github.com/JuliaPackaging/Yggdrasil/blob/24776e4eb2865cc24363cb7ed36850c5f695c820/L/libjulia/common.jl#L7-L34
# include("../../L/libjulia/common.jl")
# return the platforms supported by libjulia
function libjulia_platforms(julia_version)
    platforms = supported_platforms()

    # skip 32bit musl builds; they fail with this error:
    #    libunwind.so.8: undefined reference to `setcontext'
    filter!(p -> !(Sys.islinux(p) && libc(p) == "musl" && arch(p) == "i686"), platforms)

    if julia_version < v"1.7"
        # In Julia <= 1.6, skip macOS on ARM and Linux on armv6l
        filter!(p -> !(Sys.isapple(p) && arch(p) == "aarch64"), platforms)
        filter!(p -> arch(p) != "armv6l", platforms)
    end

    for p in platforms
        p["julia_version"] = string(julia_version)
    end

    # While the "official" Julia kernel ABI itself does not involve any C++
    # symbols on the linker level, `libjulia` still exports "unofficial" symbols
    # dependent on the C++ strings ABI (coming from LLVM related code). This
    # doesn't matter if the client code is pure C, but as soon as there are
    # other (actual) C++ dependencies, we must make sure to use the matching C++
    # strings ABI. Hence we must use `expand_cxxstring_abis` below.
    platforms = expand_cxxstring_abis(platforms)

    return platforms
end
platforms = vcat(libjulia_platforms.(julia_versions)...)

# The products that we will ensure are always built
products = [
    LibraryProduct("libbox2d_julia", :libbox2d_julia),
]

# Dependencies that must be installed before this package can be built
dependencies = [
    # https://github.com/JuliaBinaryWrappers/libjulia_jll.jl
    BuildDependency("libjulia_jll"),
    # https://github.com/JuliaBinaryWrappers/libcxxwrap_julia_jll.jl
    Dependency("libcxxwrap_julia_jll"),
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
    preferred_gcc_version = v"9",
)
