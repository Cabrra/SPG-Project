using UnityEngine;
using System.Collections;

public class Grenade : MonoBehaviour 
{
	public float 		damage 	= 20;
	public float 		damageRadius = 5.0f;
	public float 		speed 	= 5;
	public float		detonateTime = 3.0f;	
	public GameObject 	hitEffect;
	public AudioClip 	shootSound;
	public AudioClip 	hitSound;	
	
	void Start()
	{
		rigidbody.AddForce(transform.forward * speed, ForceMode.VelocityChange);
		
		Transform rangeIndicator = transform.Find("RangeIndicator");
		float scale = 1.0f/transform.localScale.magnitude * damageRadius * 2;
		rangeIndicator.localScale =  new Vector3(scale * 2, 0.1f, scale * 2);
		
		//if shootSound is not null, play it
		if(shootSound)
			ROG.PlaySound(shootSound);
	}
	
	
	void Update()
	{
		if(detonateTime > 0)
			detonateTime -= Time.deltaTime;
		else
		{
			Destroy(gameObject);
			
			// Create hit effect and play sound
			if(hitEffect)
			{
				Instantiate(hitEffect, transform.position, Quaternion.identity);
		
				if(hitSound)
					ROG.PlaySound(hitSound);							
			}
			
			ROG.AOE_Damage(transform.position, damage, damageRadius);
		}
	}
}