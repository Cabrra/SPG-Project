using UnityEngine;
using System.Collections;

public class Ninja_throw : MonoBehaviour
{
	public int Level = 1;
    public int Health = 100;
	public int MaxHealth = 100;
    public float movespeed = 5;
    public float meleeRange = 2;
    public float meleeCooldown = 2;
    public float score = 5;
    public GameObject target;
    float nextAttack = 0;
    // assets
	public AudioClip shuriken;
    public AudioClip attacksound;
    public AudioClip takeDamage;
    public AudioClip deathSound;
    public GameObject takeDamageEffect;
    public GameObject deathEffect;
    public GameObject bullet;

	public GameObject leveluppoint;
	public GameObject leveluppoint2;

    Vector3 moveDicretion;
    Vector3 dest;
    CharacterController controler;
    // Use this for initialization
    void Start () 
    {
        controler = gameObject.GetComponent<CharacterController> ();
		Health = MaxHealth;
        MaxHealth *= Level;
		
        //target = GameObject.FindGameObjectWithTag ("Player");
    }
    
    // Update is called once per frame
    void Update () 
    {
        //moveDicretion.x = Input.GetAxis ("Horizontal");
        //moveDicretion.z = Input.GetAxis ("Vertical");
        ////transform.Translate (moveDicretion * movespeed * Time.deltaTime);
        //controler.Move(moveDicretion * movespeed * Time.deltaTime);
        //  if (Input.GetKey (KeyCode.D))
        //      {
        //                      transform.Translate (movespeed * Time.deltaTime, 0, 0);
        //  }
        if (target != null)
        {
            moveDicretion = (target.transform.position - transform.position).normalized;
            Vector3 lookthisway = moveDicretion;
            lookthisway.y = 0;
            if (lookthisway.magnitude > 0) 
            {
                transform.rotation = Quaternion.LookRotation(lookthisway);      
            }
            HandleTargeting();
        }
        else
        {
            transform.Rotate(0, 90 * Time.deltaTime ,0);
        }
    }
    void HandleTargeting()
    {
        dest = target.transform.position;
        float distancetoTarget = Vector3.Distance (transform.position, target.transform.position);
        // attack?
        if (distancetoTarget < meleeRange && Time.time > nextAttack)
            Shoot ();
    }
    void Shoot()
    {
//		if(shuriken)
//			ROG.PlaySound (shuriken);
        Instantiate(bullet, transform.position + transform.forward, Quaternion.LookRotation(transform.forward)/*, transform.Rotate(Vector3.forward * Time.deltaTime * 100)*/);
        nextAttack = Time.time + meleeCooldown;
        
    }
    void Attack()
    {
        ROG.PlaySound(attacksound);
        target.SendMessage("ModifyHealth", -10 * Level, SendMessageOptions.DontRequireReceiver);
        nextAttack = Time.time + meleeCooldown;
    }

    public void ModifyHealth(int amount)
    {   
        Health += amount;
        // Optional - Draw floating text (red for damage, green for heal)
        if(amount < 0)
            new FloatingText(transform.position, (amount).ToString(), Color.red);
        else
            new FloatingText(transform.position, (amount).ToString(), Color.green);
        
        //--------------------Check death-----------------------
        if(Health < 1)
        {
            if(deathSound)
                ROG.PlaySound(deathSound);
            
            if(deathEffect)
                Instantiate(deathEffect, transform.position, transform.rotation);

			Player.Instance.SendMessage("ModifyScore", score * Level, SendMessageOptions.DontRequireReceiver);

			Destroy(gameObject);
        }   
        
        // Take damage feedback effects
        if(amount < 0)
        {
            if(takeDamageEffect)
                Instantiate(takeDamageEffect, transform.position, transform.rotation);
            
            if(takeDamage)
                ROG.PlaySound(takeDamage);
        }       
    }
}
