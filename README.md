# Box2D Julia Wrapper

Julia Wrapper for Box2D (2D rigid body simulation library for games).

https://github.com/cirocavani/Box2D_jll

https://github.com/JuliaInterop/CxxWrap.jl

https://github.com/JuliaInterop/libcxxwrap-julia


Packaging:

https://github.com/JuliaPackaging/BinaryBuilder.jl

https://binarybuilder.org/

https://docs.binarybuilder.org/stable/

> **IMPORTANT** Tested only with Julia 1.6 (other version may present issues)

```sh
# Local dev Box2D_jll.jl
cd ~/.julia/dev/Box2D_jll
git init
git add .
git commit -m "2.4.1-dev"
cd -

# cleanup
rm -rf ~/.julia/dev/Box2D_Julia_jll
rm -rf build products

# julia --project build_tarballs.jl --verbose x86_64-apple-darwin-julia_version+1.6.3
# julia --project build_tarballs.jl --deploy=cirocavani/Box2D_Julia_jll.jl
julia --project build_tarballs.jl --deploy=local --verbose x86_64-apple-darwin-julia_version+1.6.3
```

Test:

```sh
julia test/DummyBox2D.jl
```
