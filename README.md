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
rm -rf ~/.julia/dev/Box2D_Julia_jll
rm -rf build products
# julia --project build_tarballs.jl --verbose x86_64-apple-darwin
# julia --project build_tarballs.jl --deploy=local --verbose x86_64-apple-darwin
julia --project build_tarballs.jl --deploy=cirocavani/Box2D_Julia_jll.jl
```

Test:

```julia
import Pkg
Pkg.activate(; temp=true)
Pkg.develop(["Box2D_jll", "Box2D_Julia_jll"])
Pkg.add("CxxWrap")

module Box2D
    using Box2D_Julia_jll
    using CxxWrap
    @wrapmodule(libbox2d_julia)
    function __init__()
        @initcxx
    end
end

import Main.Box2D as box2d

g = box2d.Vec2()
w = box2d.World(g)
```
