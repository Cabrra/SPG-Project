using UnityEngine;
using System.Collections;

public class MeleeWeapon : MonoBehaviour 
{
	public 		float 			damage 			= 10;
	public 		float 			swingSpeed		= 360;
	public		int				direction		= 1;
	public		float			swingDegrees	= 180;
	public		Vector3			swingAxis 		= Vector3.up;
	public		bool			disableAfterHit = true;
	public		bool 			isSwinging 		= false;
	private		float			swingAmount 	= 0;
	private		Quaternion		startRotation;
	
	private		GameObject		mountPoint; 	// the parent object, this is what is actually rotated
	
	
	void Start () 
	{
		mountPoint = transform.parent.gameObject;
		startRotation = mountPoint.transform.localRotation;
		rigidbody.constraints = RigidbodyConstraints.FreezeAll;
	}
	
	
	void Update () 
	{
		if(!isSwinging && Input.GetMouseButtonDown(1))
			Swing();					

		if(isSwinging)
		{
			if(swingAmount < swingDegrees)
			{
				swingAmount += swingSpeed * Time.deltaTime;
				mountPoint.transform.Rotate(swingAxis, swingSpeed * direction * Time.deltaTime);
			}
			else
			{
				swingAmount = 0;
				mountPoint.transform.localRotation = startRotation;
				isSwinging = false;
				gameObject.SetActive(false);
			}
		}

	}
	
	
	public void Swing()
	{		
		ArmWeapon();
		isSwinging = true;	
		gameObject.SetActive(true);
	}
	
	
	// Trigger event
	void OnTriggerEnter(Collider other)
	{
		if(!isSwinging)
			return;
		
		// make sure we don't attack ourself (check 'other' ID versus this object's ID)
		if(other.gameObject.GetInstanceID() != transform.parent.root.gameObject.GetInstanceID())
		{
			//print(name + " Collision, Hit: " + other.name);
			
			other.SendMessage("ModifyHealth", -damage* Player.Instance.level, SendMessageOptions.DontRequireReceiver);
			
			// Disarm Weapon after hit
			if(disableAfterHit)
				DisArmWeapon();
		}
		if (other.tag == "Lever1" || other.tag == "Lever2" || other.tag == "lever") 
		{
			if (other.tag == "Lever1" )
			{
				if (Player.Instance.LevelUp)
					ROG.PlaySound(Player.Instance.LevelUp);
				Player.Instance.level = 2;
				new FloatingText(transform.position, "LEVEL UP" + (Player.Instance.level).ToString(), Color.yellow);
				//Player.Instance.maxhealth +=50;
				Player.Instance.health = Player.Instance.maxhealth;
				
			}
			if (other.tag == "Lever2")
			{
				if (Player.Instance.LevelUp)
					ROG.PlaySound(Player.Instance.LevelUp);
				Player.Instance.level = 2;
				new FloatingText(transform.position, "LEVEL UP" + (Player.Instance.level).ToString(), Color.yellow);
				//Player.Instance.maxhealth +=50;
				Player.Instance.health = Player.Instance.maxhealth;
			}
		}
	}
	
	// Arm Weapon (adds a mesh collider)
	public void ArmWeapon()
	{
		if(!gameObject.GetComponent<MeshCollider>())
		{
			MeshCollider mesh = gameObject.AddComponent<MeshCollider>();
			mesh.isTrigger = true;
		}

	}
	
	public void DisArmWeapon()
	{
		MeshCollider mesh = gameObject.GetComponent<MeshCollider>();
	
		if(mesh)
			Destroy(mesh);		
	}
	
}
