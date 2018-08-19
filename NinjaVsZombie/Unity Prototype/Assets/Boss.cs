using UnityEngine;
using System.Collections;

public class Boss : MonoBehaviour {
    public int Health = 100;
    public float movespeed = 20;
    public float meleeRange = 50;
    public float meleeCooldown = 2;
    public float score = 250;
    public GameObject target;
	public GameObject Pizza;
    float nextAttack = 0;
	private bool havePizza;
	public float stealtime = 1.5f;
	bool starttimer = false;
    // assets
    public AudioClip attacksound;
    public AudioClip takeDamage;
    public AudioClip deathSound;
    public GameObject takeDamageEffect;
    public GameObject deathEffect;
    GameObject inview;
    
	public float distancetoTarget2;
	public float distancetoTarget1;
    
    Vector3 moveDicretion;
    Vector3 dest;
    CharacterController controler;
    // Use this for initialization
    void Start () 
    {
		controler = gameObject.GetComponent<CharacterController> ();
		GameObject target1 = GameObject.FindGameObjectWithTag ("Pizza"); 
		GameObject target2 = GameObject.FindGameObjectWithTag ("Player");            
		float distancetoTarget2 = Vector3.Distance (transform.position, target2.transform.position);      
		float distancetoTarget1 = distancetoTarget2 + 50;
		if(target1 != null)
			distancetoTarget1 = Vector3.Distance (transform.position, target1.transform.position); 
		
		if (distancetoTarget1 < distancetoTarget2)
		{
			target = GameObject.FindGameObjectWithTag("Pizza"); 
		} 
		else if (distancetoTarget1 >= distancetoTarget2 || target1 == null)
		{
			target = GameObject.FindGameObjectWithTag("Player"); 
		}
    }
    
    // Update is called once per frame
    void Update () 
    {
        if (target != null)
        {
            moveDicretion = (target.transform.position - transform.position).normalized;
            Move();
            HandleTargeting();
        }
		if (starttimer == true)
		{
			if (stealtime > 0)
				stealtime -= Time.deltaTime;
		}
    }
    void HandleTargeting()
    {
        GameObject target1 = GameObject.FindGameObjectWithTag ("Pizza"); 
        GameObject target2 = GameObject.FindGameObjectWithTag ("Player");            
        distancetoTarget2 = Vector3.Distance (transform.position, target2.transform.position);      
        distancetoTarget1 = distancetoTarget2 + 50;
        if(target1 != null)
             distancetoTarget1 = Vector3.Distance (transform.position, target1.transform.position); 
        
        if (distancetoTarget1 < distancetoTarget2)
        {
            target = GameObject.FindGameObjectWithTag("Pizza"); 
        } 
        else if (distancetoTarget1 >= distancetoTarget2 || target1 == null)
        {
            target = GameObject.FindGameObjectWithTag("Player"); 
        }       
        dest = target.transform.position;
        float distancetoTarget = Vector3.Distance (transform.position, target.transform.position);
		if (distancetoTarget < meleeRange && Time.time > nextAttack && target == GameObject.FindGameObjectWithTag("Pizza"))
			HavePizza();
        // attack?
        if (distancetoTarget < meleeRange && Time.time > nextAttack)
            Attack ();
    }
    void Attack()
    {
        ROG.PlaySound(attacksound);
        target.SendMessage("ModifyHealth", -10, SendMessageOptions.DontRequireReceiver);
        nextAttack = Time.time + meleeCooldown;
    }
    void Move()
    {
        // rotation
        Vector3 lookthisway = moveDicretion;
        lookthisway.y = 0;
        if (lookthisway.magnitude > 0) 
        {
            transform.rotation = Quaternion.LookRotation(lookthisway);      
        }
        
        // moving
        controler.Move(moveDicretion * movespeed * Time.deltaTime);
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

			if (Pizza)
				Instantiate(Pizza, transform.position + transform.forward * 1, transform.rotation);
			havePizza = false;
			Player.Instance.SendMessage("ModifyScore", score, SendMessageOptions.DontRequireReceiver);
			Application.LoadLevel(8);
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
	void HavePizza()
	{
		if (havePizza == false)
		{
			starttimer = true;
			new FloatingText(transform.position, "Stealing pizza", Color.magenta);
			// stealtime -= Time.deltaTime;
			if (stealtime <= 0)
			{
				havePizza = true;
				GameObject.DestroyObject(GameObject.FindGameObjectWithTag("Pizza"));
				Application.LoadLevel(7); // gameover
			}
		}
	}
}
