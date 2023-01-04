
Collider::resolveCollision(Collider& collider1, Collider& collider2)
{
	if (collider1.isStatic() && collider2.isStatic())
		return;
}

Collider::resolveCollisionWithDynamic(Collider& dynamicCollider1, Collider& dynamicCollider2)
{

}

Collider::resolveCollisionWithStatic(Collider& staticCollider, Collider& dynamicCollider)
{

}
