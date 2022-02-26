#include "box2d/box2d.h"
#include "jlcxx/jlcxx.hpp"
#include "jlcxx/functions.hpp"
#include "jlcxx/array.hpp"

enum ContactEvent {
  BEGIN_CONTACT = 0,
  END_CONTACT
};

struct ContactListenerAdapter : public b2ContactListener
{
  void(*_callback)(ContactEvent, b2Contact*);

  ContactListenerAdapter(void(*callback)(ContactEvent, b2Contact*)) : _callback(callback) {}
  ~ContactListenerAdapter() { _callback = nullptr; }

  void BeginContact(b2Contact* contact)
  {
    _callback(BEGIN_CONTACT, contact);
  }

  void EndContact(b2Contact* contact)
  {
    _callback(END_CONTACT, contact);
  }
};

namespace jlcxx
{
  template<> struct SuperType<b2PolygonShape> { typedef b2Shape type; };
  template<> struct SuperType<b2CircleShape> { typedef b2Shape type; };
  template<> struct SuperType<b2EdgeShape> { typedef b2Shape type; };
  template<> struct SuperType<b2ChainShape> { typedef b2Shape type; };

  template<> struct SuperType<b2DistanceJointDef> { typedef b2JointDef type; };
  template<> struct SuperType<b2FrictionJointDef> { typedef b2JointDef type; };
  template<> struct SuperType<b2GearJointDef> { typedef b2JointDef type; };
  template<> struct SuperType<b2MotorJointDef> { typedef b2JointDef type; };
  template<> struct SuperType<b2MouseJointDef> { typedef b2JointDef type; };
  template<> struct SuperType<b2PrismaticJointDef> { typedef b2JointDef type; };
  template<> struct SuperType<b2PulleyJointDef> { typedef b2JointDef type; };
  template<> struct SuperType<b2RevoluteJointDef> { typedef b2JointDef type; };
  template<> struct SuperType<b2WeldJointDef> { typedef b2JointDef type; };
  template<> struct SuperType<b2WheelJointDef> { typedef b2JointDef type; };

  template<> struct SuperType<b2DistanceJoint> { typedef b2Joint type; };
  template<> struct SuperType<b2FrictionJoint> { typedef b2Joint type; };
  template<> struct SuperType<b2GearJoint> { typedef b2Joint type; };
  template<> struct SuperType<b2MotorJoint> { typedef b2Joint type; };
  template<> struct SuperType<b2MouseJoint> { typedef b2Joint type; };
  template<> struct SuperType<b2PrismaticJoint> { typedef b2Joint type; };
  template<> struct SuperType<b2PulleyJoint> { typedef b2Joint type; };
  template<> struct SuperType<b2RevoluteJoint> { typedef b2Joint type; };
  template<> struct SuperType<b2WeldJoint> { typedef b2Joint type; };
  template<> struct SuperType<b2WheelJoint> { typedef b2Joint type; };

  template<> struct SuperType<ContactListenerAdapter> { typedef b2ContactListener type; };
}

JLCXX_MODULE define_julia_module(jlcxx::Module& mod)
{
  mod.add_type<b2Vec2>("Vec2")
  .constructor()
  .constructor<float, float>()
  .method("get_x", [](const b2Vec2& v) { return v.x; })
  .method("get_y", [](const b2Vec2& v) { return v.y; })
  .method("coordinates!", &b2Vec2::Set)
  .method("zero!", &b2Vec2::SetZero)
  .method("neg", [](const b2Vec2& v) { return -v; })
  .method("plus!", [](b2Vec2& v1, const b2Vec2& v2) { v1 += v2; })
  .method("minus!", [](b2Vec2& v1, const b2Vec2& v2) { v1 -= v2; })
  .method("scale!", [](b2Vec2& v, float a) { v *= a; })
  .method("dot", [](b2Vec2& v1, const b2Vec2& v2) { return b2Dot(v1, v2); })
  .method("length", &b2Vec2::Length)
  .method("length_squared", &b2Vec2::LengthSquared)
  .method("normalize!", &b2Vec2::Normalize)
  .method("is_valid", &b2Vec2::IsValid)
  .method("skew", &b2Vec2::Skew)
  ;

  mod.add_type<b2MassData>("MassData")
  .method("mass", [](b2MassData& massData) { return massData.mass; })
  .method("center", [](b2MassData& massData) { return massData.center; })
  .method("inertia", [](b2MassData& massData) { return massData.I; })
  ;

  mod.add_type<b2Shape>("Shape")
  .method("radius!", [](b2Shape& shape, float radius) { shape.m_radius = radius; })
  .method("radius", [](b2Shape& shape) { return shape.m_radius; })
  .method("compute_mass", &b2Shape::ComputeMass)
  ;

  mod.add_type<b2PolygonShape>("PolygonShape", jlcxx::julia_base_type<b2Shape>())
  .method(
    "make_box!",
    [](b2PolygonShape& shape, float hx, float hy) {
      shape.SetAsBox(hx, hy);
    }
  )
  .method(
    "make_box!",
    [](b2PolygonShape& shape, float hx, float hy, const b2Vec2 &center, float angle) {
      shape.SetAsBox(hx, hy, center, angle);
    }
  )
  .method(
    "vertices!",
    [](b2PolygonShape& shape, jlcxx::ArrayRef<jl_value_t*> vertices) {
      b2Vec2* points = new b2Vec2[vertices.size()];
      for (size_t i = 0; i < vertices.size(); ++i) {
        points[i] = jlcxx::unbox<b2Vec2>(vertices[i]);
      }
      shape.Set(points, vertices.size());
      delete [] points;
    }
  )
  .method("centroid", [](b2PolygonShape& shape) -> b2Vec2& { return shape.m_centroid; })
  ;
  mod.add_type<b2CircleShape>("CircleShape", jlcxx::julia_base_type<b2Shape>());
  mod.add_type<b2EdgeShape>("EdgeShape", jlcxx::julia_base_type<b2Shape>());
  mod.add_type<b2ChainShape>("ChainShape", jlcxx::julia_base_type<b2Shape>());

  mod.add_type<b2FixtureDef>("FixtureDef")
  .constructor()
  .method("shape!", [](b2FixtureDef& fixture_def, const b2Shape* shape) { fixture_def.shape = shape; })
  .method("shape", [](b2FixtureDef& fixture_def) { return fixture_def.shape; })
  .method("density!", [](b2FixtureDef& fixture_def, float density) { fixture_def.density = density; })
  .method("density", [](b2FixtureDef& fixture_def) { return fixture_def.density; })
  .method("friction!", [](b2FixtureDef& fixture_def, float friction) { fixture_def.friction = friction; })
  .method("friction", [](b2FixtureDef& fixture_def) { return fixture_def.friction; })
  .method(
    "categorybits!",
    [](b2FixtureDef& fixture_def, jlcxx::StrictlyTypedNumber<uint16> bits) {
      fixture_def.filter.categoryBits = bits.value;
    }
  )
  .method(
    "categorybits",
    [](b2FixtureDef& fixture_def) { return fixture_def.filter.categoryBits; }
  )
  .method(
    "maskbits!",
    [](b2FixtureDef& fixture_def, jlcxx::StrictlyTypedNumber<uint16> bits) {
      fixture_def.filter.maskBits = bits.value;
    }
  )
  .method(
    "maskbits",
    [](b2FixtureDef& fixture_def) { return fixture_def.filter.maskBits; }
  )
  .method(
    "groupindex!",
    [](b2FixtureDef& fixture_def, jlcxx::StrictlyTypedNumber<int16> index) {
      fixture_def.filter.groupIndex = index.value;
    }
  )
  .method(
    "groupindex",
    [](b2FixtureDef& fixture_def) { return fixture_def.filter.groupIndex; }
  )
  ;

  mod.add_type<b2Fixture>("Fixture");

  mod.add_bits<b2BodyType>("BodyType", jlcxx::julia_type("CppEnum"));
  mod.set_const("STATIC_BODY", b2_staticBody);
  mod.set_const("KINEMATIC_BODY", b2_kinematicBody);
  mod.set_const("DYNAMIC_BODY", b2_dynamicBody);

  mod.add_type<b2BodyDef>("BodyDef")
  .constructor()
  .method("make_static!", [](b2BodyDef& body_def) { body_def.type = b2_staticBody; })
  .method("make_kinematic!", [](b2BodyDef& body_def) { body_def.type = b2_kinematicBody; })
  .method("make_dynamic!", [](b2BodyDef& body_def) { body_def.type = b2_dynamicBody; })
  .method("body_type", [](b2BodyDef& body_def) -> b2BodyType { return body_def.type; })
  .method("position", [](b2BodyDef& body_def) -> b2Vec2& { return body_def.position; })
  ;

  mod.add_type<b2Body>("Body")
  .method(
    "create_fixture",
    [](b2Body* body, const b2FixtureDef* def) { return body->CreateFixture(def); }
  )
  .method(
    "create_fixture",
    [](b2Body* body, const b2Shape* shape, float density) { return body->CreateFixture(shape, density); }
  )
  .method("destroy_fixture", &b2Body::DestroyFixture)
  .method("get_position", &b2Body::GetPosition)
  .method("get_angle", &b2Body::GetAngle)
  .method("transform!", &b2Body::SetTransform)
  ;

  mod.add_type<b2JointDef>("JointDef");
  mod.add_type<b2DistanceJointDef>("DistanceJointDef", jlcxx::julia_base_type<b2JointDef>())
  .method("initialize!", &b2DistanceJointDef::Initialize)
  .method("length", [](b2DistanceJointDef& joint_def) { return joint_def.length; })
  .method("min_length!", [](b2DistanceJointDef& joint_def, float length) { joint_def.minLength = length; })
  .method("max_length!", [](b2DistanceJointDef& joint_def, float length) { joint_def.maxLength = length; })
  ;
  mod.add_type<b2FrictionJointDef>("FrictionJointDef", jlcxx::julia_base_type<b2JointDef>())
  // .method("initialize!", &b2FrictionJointDef::Initialize)
  ;
  mod.add_type<b2GearJointDef>("GearJointDef", jlcxx::julia_base_type<b2JointDef>());
  mod.add_type<b2MotorJointDef>("MotorJointDef", jlcxx::julia_base_type<b2JointDef>())
  // .method("initialize!", &b2MotorJointDef::Initialize)
  ;
  mod.add_type<b2MouseJointDef>("MouseJointDef", jlcxx::julia_base_type<b2JointDef>());
  mod.add_type<b2PrismaticJointDef>("PrismaticJointDef", jlcxx::julia_base_type<b2JointDef>())
  .method("initialize!", &b2PrismaticJointDef::Initialize)
  ;
  mod.add_type<b2PulleyJointDef>("PulleyJointDef", jlcxx::julia_base_type<b2JointDef>())
  // .method("initialize!", &b2PulleyJointDef::Initialize)
  ;
  mod.add_type<b2RevoluteJointDef>("RevoluteJointDef", jlcxx::julia_base_type<b2JointDef>())
  .method("initialize!", &b2RevoluteJointDef::Initialize)
  ;
  mod.add_type<b2WeldJointDef>("WeldJointDef", jlcxx::julia_base_type<b2JointDef>())
  // .method("initialize!", &b2WeldJointDef::Initialize)
  ;
  mod.add_type<b2WheelJointDef>("WheelJointDef", jlcxx::julia_base_type<b2JointDef>())
  // .method("initialize!", &b2WheelJointDef::Initialize)
  ;

  mod.add_type<b2Joint>("Joint")
  .method("get_reaction_force", &b2Joint::GetReactionForce)
  .method("get_reaction_torque", &b2Joint::GetReactionTorque)
  ;

  mod.add_type<b2DistanceJoint>("DistanceJoint", jlcxx::julia_base_type<b2Joint>());
  mod.add_type<b2FrictionJoint>("FrictionJoint", jlcxx::julia_base_type<b2Joint>());
  mod.add_type<b2GearJoint>("GearJoint", jlcxx::julia_base_type<b2Joint>());
  mod.add_type<b2MotorJoint>("MotorJoint", jlcxx::julia_base_type<b2Joint>());
  mod.add_type<b2MouseJoint>("MouseJoint", jlcxx::julia_base_type<b2Joint>());
  mod.add_type<b2PrismaticJoint>("PrismaticJoint", jlcxx::julia_base_type<b2Joint>());
  mod.add_type<b2PulleyJoint>("PulleyJoint", jlcxx::julia_base_type<b2Joint>());
  mod.add_type<b2RevoluteJoint>("RevoluteJoint", jlcxx::julia_base_type<b2Joint>());
  mod.add_type<b2WeldJoint>("WeldJoint", jlcxx::julia_base_type<b2Joint>());
  mod.add_type<b2WheelJoint>("WheelJoint", jlcxx::julia_base_type<b2Joint>());

  mod.add_type<b2Contact>("Contact");

  mod.add_type<b2ContactListener>("ContactListener");

  mod.add_bits<ContactEvent>("ContactEvent", jlcxx::julia_type("CppEnum"));
  mod.set_const("BEGIN_CONTACT", BEGIN_CONTACT);
  mod.set_const("END_CONTACT", END_CONTACT);

  mod.add_type<ContactListenerAdapter>("ContactListenerAdapter", jlcxx::julia_base_type<b2ContactListener>())
  .constructor<void(ContactEvent, b2Contact*)>();

  mod.add_type<b2World>("World")
  .constructor<const b2Vec2&>()
  .method("create_body", &b2World::CreateBody)
  .method("destroy_body", &b2World::DestroyBody)
  .method("create_joint", &b2World::CreateJoint)
  .method("destroy_joint", &b2World::DestroyJoint)
  .method("contact_listener!", &b2World::SetContactListener)
  .method("step!", &b2World::Step)
  ;
}
