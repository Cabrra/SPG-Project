using UnityEngine;
using System.Collections;

public class Hookbehaviour1 : MonoBehaviour {
    public float        damage  = 0;
    public float        speed   = 30;
    
    public GameObject   hitEffect;
    public AudioClip    shootSound;
    public AudioClip    hitSound;   
    
    private LineRenderer line;
    void Start()
    {
        // add a force to the rigidbody
        rigidbody.AddForce(transform.forward * speed * Player.Instance.level, ForceMode.VelocityChange);
        
        // Mark bullet for deletion 5 seconds after creation
        Destroy(gameObject, 0.5f);  

        line = this.GetComponent<LineRenderer>();
        line.SetPosition(0, this.transform.position);
        line.SetPosition(1, Player.Instance.transform.position);

        //if shootSound is not null, play it
        if(shootSound)
            ROG.PlaySound(shootSound);
    }

    void Update()
    {
        line.SetPosition(0, this.transform.position);
        line.SetPosition(1, Player.Instance.transform.position);

    }

    // Collision event
    void OnTriggerEnter(Collider other)
    {
        
        //      print(gameObject.name + " Collision, Hit: " + other.name);
        
        // Destroy the bullet
        
        
        if (other.tag == "Pizza")
		{
			if (Player.Instance.TakePizza)
				ROG.PlaySound(Player.Instance.TakePizza);
			Player.Instance.havePizza = true;
			Destroy(gameObject);
		}
		else if (other.tag == "Lever1" || other.tag == "Lever2" || other.tag == "lever") 
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
			Destroy(gameObject);
		}
        // Send a message to the other object (call ModifyHealth() if it exists)
        //other.SendMessage("ModifyHealth", -damage, SendMessageOptions.DontRequireReceiver);
        
        // Create hit effect and play sound
        if(hitEffect)
        {
            Instantiate(hitEffect, transform.position, transform.rotation);
            
            if(hitSound)
                ROG.PlaySound(hitSound);                            
        }
    }

    // Collision event
    void OnCollisionEnter(Collision collisionData)
    {
        // Get the object that was hit
        GameObject other = collisionData.gameObject;    
        
        //      print(gameObject.name + " Collision, Hit: " + other.name);
        

        
        if (other.tag == "Pizza")
		{
            Player.Instance.havePizza = true;
		}
			else if (other.tag == "Immovable")
        {
            Vector3 temppos = other.collider.ClosestPointOnBounds(Player.Instance.transform.position);
            Vector3 tempdir = (temppos - transform.position).normalized;
            Player.Instance.transform.position = temppos-tempdir*0.6f;
        }
		else if (other.tag == "Movable")
		{
			Vector3 temppos = Player.Instance.collider.ClosestPointOnBounds(other.transform.position);
			Vector3 tempdir = (temppos - transform.position).normalized;
			other.transform.position = temppos-tempdir*1.5f;
		}
        // Destroy the bullet
        Destroy(gameObject);

        // Send a message to the other object (call ModifyHealth() if it exists)
        //other.SendMessage("ModifyHealth", -damage, SendMessageOptions.DontRequireReceiver);
        
        // Create hit effect and play sound
        if(hitEffect)
        {
            Instantiate(hitEffect, transform.position, transform.rotation);
            
            if(hitSound)
                ROG.PlaySound(hitSound);                            
        }
    }
}
