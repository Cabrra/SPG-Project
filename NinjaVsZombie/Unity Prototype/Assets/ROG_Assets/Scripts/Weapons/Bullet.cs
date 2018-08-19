using UnityEngine;
using System.Collections;

public class Bullet : MonoBehaviour 
{
	public float 		damage 	= 20;
	public float 		speed 	= 30;
	
	public GameObject 	hitEffect;
	public AudioClip 	shootSound;
	public AudioClip 	hitSound;	
	
	
	void Start()
	{
		// add a force to the rigidbody
		rigidbody.AddForce(transform.forward * speed, ForceMode.VelocityChange);
		
		// Mark bullet for deletion 5 seconds after creation
		Destroy(gameObject, 5);		
		
		//if shootSound is not null, play it
		if(shootSound)
			ROG.PlaySound(shootSound);
	}
	
	
	// Collision event
	void OnCollisionEnter(Collision collisionData)
	{
		// Get the object that was hit
		GameObject other = collisionData.gameObject;	
		
//		print(gameObject.name + " Collision, Hit: " + other.name);
				
		// Destroy the bullet
		Destroy(gameObject);
		
		//if(other.tag == "Enemy")
			//other.GetComponent<_Enemy>().health -= damage;
		
		// Send a message to the other object (call ModifyHealth() if it exists)
		other.SendMessage("ModifyHealth", -damage, SendMessageOptions.DontRequireReceiver);

		// Create hit effect and play sound
		if(hitEffect)
		{
			Instantiate(hitEffect, transform.position, transform.rotation);
	
			if(hitSound)
				ROG.PlaySound(hitSound);							
		}
	}
}