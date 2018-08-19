using UnityEngine;
using System.Collections;

public class DestructableObject : MonoBehaviour 
{
	public float 		health = 1;
	public GameObject 	destroyEffect;
	public AudioClip	destroySound;
	public float		AOE_DamageRadius = 0;
	public float		AOE_Damage = 0;

	void Update () 
	{
		if(health < 1)
		{
			if(destroyEffect != null)
				Instantiate(destroyEffect, transform.position, transform.rotation);
			
			if(destroySound != null)
				ROG.PlaySound(destroySound);
			
			if(AOE_DamageRadius > 0)
				ROG.AOE_Damage(transform.position, AOE_Damage, AOE_DamageRadius);
			
			Destroy(gameObject);
		}
	}
	
	
	public void ModifyHealth(float amount)
	{
		health += amount;	
	}
}
