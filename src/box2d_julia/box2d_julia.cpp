#include "box2d/box2d.h"
#include "jlcxx/jlcxx.hpp"
#include "jlcxx/functions.hpp"

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
  .method("scale!", [](b2Vec2& v1, float a) { v1 *= a; })
  .method("length", &b2Vec2::Length)
  .method("length_squared", &b2Vec2::LengthSquared)
  .method("normalize!", &b2Vec2::Normalize)
  .method("is_valid", &b2Vec2::IsValid)
  .method("skew", &b2Vec2::Skew)
  ;

  mod.add_type<b2Shape>("Shape");

  mod.add_type<b2PolygonShape>("PolygonShape", jlcxx::julia_base_type<b2Shape>())
  .constructor();

  mod.add_type<b2CircleShape>("CircleShape", jlcxx::julia_base_type<b2Shape>())
  .constructor();

  mod.method("radius!", [](b2Shape& shape, float radius) { shape.m_radius = radius; });
  mod.method("radius", [](b2Shape& shape) { return shape.m_radius; });
  mod.method("make_box!", [](b2PolygonShape& shape, float hx, float hy) { shape.SetAsBox(hx, hy); });

  mod.add_type<b2FixtureDef>("FixtureDef")
  .constructor();

  mod.method("shape!", [](b2FixtureDef& fixture_def, const b2Shape* shape) { fixture_def.shape = shape; });
  mod.method("shape", [](b2FixtureDef& fixture_def) { return fixture_def.shape; });
  mod.method("density!", [](b2FixtureDef& fixture_def, float density) { fixture_def.density = density; });
  mod.method("density", [](b2FixtureDef& fixture_def) { return fixture_def.density; });
  mod.method("friction!", [](b2FixtureDef& fixture_def, float friction) { fixture_def.friction = friction; });
  mod.method("friction", [](b2FixtureDef& fixture_def) { return fixture_def.friction; });

  mod.add_type<b2Fixture>("Fixture");

  mod.add_bits<b2BodyType>("BodyType", jlcxx::julia_type("CppEnum"));
  mod.set_const("STATIC_BODY", b2_staticBody);
  mod.set_const("KINEMATIC_BODY", b2_kinematicBody);
  mod.set_const("DYNAMIC_BODY", b2_dynamicBody);

  mod.add_type<b2BodyDef>("BodyDef")
  .constructor();

  mod.method("make_static!", [](b2BodyDef& body_def) { body_def.type = b2_staticBody; });
  mod.method("make_kinematic!", [](b2BodyDef& body_def) { body_def.type = b2_kinematicBody; });
  mod.method("make_dynamic!", [](b2BodyDef& body_def) { body_def.type = b2_dynamicBody; });
  mod.method("body_type", [](b2BodyDef& body_def) -> b2BodyType { return body_def.type; });
  mod.method("position", [](b2BodyDef& body_def) -> b2Vec2& { return body_def.position; });

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
  .method("set_transform", &b2Body::SetTransform)
  ;

  mod.add_type<b2JointDef>("JointDef");

  mod.add_type<b2Joint>("Joint");

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
