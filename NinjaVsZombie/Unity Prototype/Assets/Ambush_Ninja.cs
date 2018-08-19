using UnityEngine;
using System.Collections;

public class Ambush_Ninja : MonoBehaviour 
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
    public bool havePizza = false;
    public float stealtime = 1.5f;
    public float lifetime = 6f;
    public float gameovertimer = 8;
	public GameObject leveluppoint;
	public GameObject leveluppoint2;
	public Vector3 move = Vector3.zero;
    bool starttimer = false;
    NavMeshAgent agent;
    
    // assets
    //public AudioClip attacksound;
	public AudioClip takeDamage;
	public AudioClip deathSound;
	public AudioClip pizzaTake;
	public AudioClip pizzaDrop;

    public GameObject takeDamageEffect;
    public GameObject deathEffect;
	public GameObject Pizza;
    GameObject inview;
    GameObject player;
    
    Vector3 moveDicretion;
    Vector3 dest;
    CharacterController controler;
    // Use this for initialization
    void Start () 
    {
        controler = gameObject.GetComponent<CharacterController> ();
        // inview = gameObject.GetComponent<Zoombie>();      
        dest = transform.position;
        target = GameObject.FindGameObjectWithTag("Pizza");
        Health = MaxHealth;
        player = GameObject.FindGameObjectWithTag("Player");
        MaxHealth = MaxHealth * Level;
        agent = this.GetComponent<NavMeshAgent>();
        
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
        //        if (inview.CompareTag("Player"))
        //        {   
        //        target = GameObject.FindGameObjectWithTag ("Player");
        //}
        Debug.DrawLine(transform.position, agent.destination);
        agent.destination = dest;
        moveDicretion = (dest - transform.position).normalized;
        if (havePizza)
        {
//            moveDicretion = (transform.position - player.transform.position).normalized;
//            gameovertimer -= Time.deltaTime;
//            if (gameovertimer <= 0)
//            {
//                Application.LoadLevel(0);
//            }
            Vector3 pos = Camera.main.WorldToScreenPoint(transform.position);
            Rect R = new Rect(0,0,1024,768);
            if (!R.Contains(pos)) {
                Application.LoadLevel(7);
            }
            
        } 
        if (target != null)
        {
            
            dest = target.transform.position;
            
            float distancetoTarget = Vector3.Distance(transform.position, target.transform.position);
            //take the pizza
            //if (havePizza) {
            //  target = ROG.FindNearestObjectByTag(transform.position, "RunAway");
            //}
           if (distancetoTarget < meleeRange && Time.time > nextAttack && target == GameObject.FindGameObjectWithTag("Pizza"))
              HavePizza();
            // attack?
            //if (distancetoTarget < meleeRange && Time.time > nextAttack)
            //    Attack();
            
            //HandleTargeting ();
        } 
        else
        {
            float distance = Vector3.Distance(transform.position, dest);
            if (distance < 1) 
            {
                Vector3 temp = transform.position + new Vector3(Random.Range(-2, 2), 0, Random.Range(-2, 2));
                if (ROG.hasLOS(transform.position, temp)) 
                {
                    dest = temp;
                }
            }
        }
        
        if (havePizza == false)
        {
            lifetime -= Time.deltaTime;
            if (lifetime <= 0 ) 
            {
                Destroy(gameObject);
            }
        }
        
        if (starttimer == true)
        {
            if (stealtime > 0)
                stealtime -= Time.deltaTime;
        }
        // HavePizza();
        //Move();

		if(controler.isGrounded == false)
		{
			move.y -= Player.Instance.gravity * Time.deltaTime;

			if (CamControl.Instance.transform.position.x == 0 && move.y <= -1)
			{
				if (transform.position.y < -1)
					transform.localScale *= 0.9f; //= new Vector3(0.5f , 1 ,0.5f);
//				else if (this.transform.position.y < -30)
//				{
//					if (deathSound)
//						ROG.PlaySound(deathSound);
//					Destroy();
//				}
			}
		}
		controler.Move(move * Time.deltaTime);
        
    }
    void HandleTargeting()
    {
    }
   // void Attack()
   // {
   //     ROG.PlaySound(attacksound);
   //     target.SendMessage("ModifyHealth", -10, SendMessageOptions.DontRequireReceiver);
   //     nextAttack = Time.time + meleeCooldown;
   // }
//    void Move()
//    {
//        // rotation
//        Vector3 lookthisway = moveDicretion;
//        lookthisway.y = 0;
//        if (lookthisway.magnitude > 0) 
//        {
//            transform.rotation = Quaternion.LookRotation(lookthisway);      
//        }
//        
//        
//        // moving
//        controler.Move(moveDicretion * movespeed * Time.deltaTime);
//    }
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
			if (Pizza && havePizza)
			{
				if (pizzaDrop)
					ROG.PlaySound(pizzaDrop);
				Instantiate(Pizza, transform.position + transform.forward * 1, transform.rotation);
			}
			havePizza = false;

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
    
    void HavePizza()
    {
        if (havePizza == false)
        {
           
            starttimer = true;
            // stealtime -= Time.deltaTime;
            if (stealtime <= 0)
            {
				if (pizzaTake)
					ROG.PlaySound(pizzaTake);
                havePizza = true;
                GameObject.DestroyObject(GameObject.FindGameObjectWithTag("Pizza"));
            }
            GameObject[] rooms = GameObject.FindGameObjectsWithTag("Room");
            GameObject newroom = rooms[Random.Range(0, rooms.Length)];
            dest = new Vector3(newroom.transform.position.x, 1.2f, newroom.transform.position.z);
            
        }
//        if (havePizza == true)
//        {
//            // smart ai
//           GameObject target1 = GameObject.FindGameObjectWithTag ("RunAway1"); 
//           GameObject target2 = GameObject.FindGameObjectWithTag ("RunAway2");            
//           GameObject target3 = GameObject.FindGameObjectWithTag ("RunAway3");            
//           GameObject target4 = GameObject.FindGameObjectWithTag ("RunAway4");            
//           float distancetoTarget1 = Vector3.Distance (transform.position, target1.transform.position); 
//           float distancetoTarget2 = Vector3.Distance (transform.position, target2.transform.position);            
//           float distancetoTarget3 = Vector3.Distance (transform.position, target3.transform.position);            
//           float distancetoTarget4 = Vector3.Distance (transform.position, target4.transform.position); 
//
//
//            if (distancetoTarget1 < distancetoTarget2 && distancetoTarget1 < distancetoTarget3 && distancetoTarget1 < distancetoTarget4) 
//            {
//                target = GameObject.FindGameObjectWithTag ("RunAway1");
//            }
//            else if (distancetoTarget2 < distancetoTarget1 && distancetoTarget2 < distancetoTarget3 && distancetoTarget2 < distancetoTarget4) 
//            {
//                target = GameObject.FindGameObjectWithTag ("RunAway2");
//            }
//           else if (distancetoTarget3 < distancetoTarget2 && distancetoTarget3 < distancetoTarget1 && distancetoTarget3 < distancetoTarget4) 
//            {
//                target = GameObject.FindGameObjectWithTag ("RunAway3");
//            }
//            else 
//            {
//                target = GameObject.FindGameObjectWithTag ("RunAway4");
//            }
            // Random random = new Random();
            
            // int randomNumber = random.Next(1, 5);
       //    if(settargetonce == true)
       //    {
       //        int randomNumber = Random.Range(1, 5);
       //        if (randomNumber == 1) 
       //        {
       //            target = GameObject.FindGameObjectWithTag ("RunAway1");
       //        }
       //        else if (randomNumber == 2) 
       //        {
       //            target = GameObject.FindGameObjectWithTag ("RunAway2");
       //        }
       //        else if (randomNumber == 3) 
       //        {
       //            target = GameObject.FindGameObjectWithTag ("RunAway3");
       //        }
       //        else if (randomNumber == 4) 
       //        {
       //            target = GameObject.FindGameObjectWithTag ("RunAway4");
       //        } 
       //        settargetonce = false;
       //    }           
        //}
    }
    
    
    
}