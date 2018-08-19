using UnityEngine;
using System.Collections;

public class Gun : BaseWeapon
{
	public	GameObject	projectile;

	
	protected override void Start()
	{
		renderer.enabled = false;
	}
	
	public override void Activate(Vector3 direction)
	{
		if(CanActivate() && projectile != null)
		{			
			// Create a bullet facing the new direction
			Quaternion dir = Quaternion.LookRotation(direction);
			GameObject bullet = (GameObject)Instantiate(projectile, transform.position, dir);
					
			Physics.IgnoreCollision(this.transform.root.collider, bullet.collider);
			
			SetNewActivateTime();
		}
	}
}
