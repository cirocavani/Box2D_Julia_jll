import Pkg
Pkg.activate(; temp=true)
Pkg.develop(["Box2D_jll", "Box2D_Julia_jll"])
Pkg.add("CxxWrap")

module DummyBox2D
    using Box2D_Julia_jll
    using CxxWrap
    @wrapmodule(libbox2d_julia)
    function __init__()
        @initcxx
    end
end

import Main.DummyBox2D as box2d

g = box2d.Vec2()
w = box2d.World(g)
