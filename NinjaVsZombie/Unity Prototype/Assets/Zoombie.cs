using UnityEngine;
using System.Collections;

public class Zoombie : MonoBehaviour
{
    public int Level = 1;
    public int MaxHealth = 100;
    public int Health = 40;
    public float movespeed = 5;
    public float meleeRange = 2;
    public float meleeCooldown = 2;
    public float score = 5;
    public GameObject target;
    float nextAttack = 0;
    public bool havePizza = false;
    public float stealtime = 1.5f;
    //bool settargetonce = true;
    public int counttobeat = 0;
    bool starttimer = false;
    public float gameovertimer = 8;
    // assets
    public AudioClip attacksound;
    public AudioClip takeDamage;
    public AudioClip deathSound;
	public AudioClip pizzaTake;
	public AudioClip pizzaDrop;
	public AudioClip fightSound;
	
	public GameObject takeDamageEffect;
    public GameObject deathEffect;
    public GameObject Pizza;
	private GameObject inview;
	private Vector3 moveDicretion;
	private Vector3 dest;
    private CharacterController controler;
    // Use this for initialization
    GameObject player;

    NavMeshAgent agent;

    void Start()
    {
        controler = gameObject.GetComponent<CharacterController>();
        Health = MaxHealth;
        stealtime = 3;
        // inview = gameObject.GetComponent<Zoombie>();
        //settargetonce = true;       
        dest = new Vector3(transform.position.x + 2, transform.position.y, transform.position.z + 2);
        player = GameObject.FindGameObjectWithTag("Player");
        agent = this.GetComponent<NavMeshAgent>();
     //   print(agent.updatePosition);
        agent.updatePosition = true;
        MaxHealth *= Level;                
		
    }
    
    // Update is called once per frame
    void Update()
    {
        //print(agent.destination);
        Debug.DrawLine(transform.position, agent.destination);
        agent.destination = dest;
		//agent.speed = 4;
        
        if (havePizza)
        {

			target = null;
            Vector3 pos = Camera.main.WorldToScreenPoint(transform.position);
            Rect R = new Rect(0,0,1024,768);
            if (!R.Contains(pos)) {
                Application.LoadLevel(7);
            }


        } 
        else 
        {

    
            if (target != null)
            {
 
                dest = new Vector3(target.transform.position.x, 1.2f, target.transform.position.z);
                //dest = target.transform.position;
            
                float distancetoTarget = Vector3.Distance(transform.position, target.transform.position);
                //take the pizza

                // attack?
                if (target.tag == "Player") 
                {
                    if (distancetoTarget < meleeRange && Time.time > nextAttack)
                    {
						agent.speed = 0;
                        Attack();
                    }
                    stealtime = 3;
                }
                else
                {
                    if (distancetoTarget < meleeRange)
                    {
                        //movespeed = 0;
                        if (stealtime > 0)
                            stealtime -= Time.deltaTime;
                        else
                            TakePizza();            
                    }
                }


                //HandleTargeting ();
            } 
			else if (Player.Instance.isAttacked == false)
            {
                float distance = Vector3.Distance(transform.position, dest);
               // print(distance);
                if (distance < 1)
                {
                    Vector3 temp = transform.position + new Vector3(Random.Range(-20, 20), 0, Random.Range(-2, 2));
                    if (ROG.hasLOS(transform.position, temp))
                    {
                        dest = temp;
                    }
                }
            }
        }
        // HavePizza();
       // Move();
        
    }

    void HandleTargeting()
    {
    }

    void Attack()
    {
//ROG.PlaySound(attacksound);
//get.SendMessage("ModifyHealth", -10 * Level, SendMessageOptions.DontRequireReceiver);
	nextAttack = Time.time + meleeCooldown;
      if (Player.Instance.havePizza == true) 
      {
			new FloatingText(transform.position, "Fight for pizza\n<press O/B>", Color.yellow);
          Player.Instance.isAttacked = true;
          
          nextAttack = Time.time + meleeCooldown;
          counttobeat++;
          if (counttobeat > 10) 
          {
			havePizza = true;
         	Player.Instance.havePizza = false;
			agent.speed = 4;
          }
          
          
          if (Player.Instance.countbeat <= 0 ) 
          {
              Player.Instance.isAttacked = false;
              Vector3 temppos = Player.Instance.collider.ClosestPointOnBounds(gameObject.transform.position);
              Vector3 tempdir = (temppos - transform.position).normalized;
              gameObject.transform.position = temppos-tempdir*3.0f;
				agent.speed = 4;
				
          }
          
      }
      else 
      {
          Player.Instance.isAttacked = false;
          counttobeat = 0;
          return;
      }
//      HavePizza();
    }

    public void ChangeTarget(GameObject newTarget)
    {
        // 0 = player, 1 = pizza, 2 = null
        //movespeed = 2.5f;
        target = newTarget;
    }
//   

    public void ModifyHealth(int amount)
    {   
        Health += amount;
        // Optional - Draw floating text (red for damage, green for heal)
        if (amount < 0)
            new FloatingText(transform.position, (amount).ToString(), Color.red);
        else
            new FloatingText(transform.position, (amount).ToString(), Color.green);
        
        //--------------------Check death-----------------------
        if (Health < 1)
        {
            if (deathSound)
                ROG.PlaySound(deathSound);
            
            if (deathEffect)
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
        if (amount < 0)
        {
            if (takeDamageEffect)
                Instantiate(takeDamageEffect, transform.position, transform.rotation);
            
            if (takeDamage)
                ROG.PlaySound(takeDamage);
        }       
    }

    void TakePizza()
    {
        if (havePizza == false)
        {
            //settargetonce = true;
            starttimer = true;
            target = null;
            // stealtime -= Time.deltaTime;
            print("took pizza");
			print(pizzaTake.ToString());
			if(pizzaTake)
				ROG.PlaySound(pizzaTake);
                havePizza = true;
                GameObject.DestroyObject(GameObject.FindGameObjectWithTag("Pizza"));
            //movespeed = 2.5f;
//            RaycastHit hit;
//            GameObject myRoom = null;
//            if(Physics.Raycast(transform.position, Vector3.down, out hit, 5))
//            {
//                myRoom = hit.collider.gameObject;
//                print("in room:" + myRoom.name);
//            }

            GameObject[] rooms = GameObject.FindGameObjectsWithTag("Room");
            GameObject newroom = rooms[Random.Range(0, rooms.Length)];
//            if(myRoom != null)
//            {
//                while(newroom.GetInstanceID() != myRoom.GetInstanceID())
//                {
//                        newroom = rooms[Random.Range(0, rooms.Length)];
//                }
//            }
            //print("old:" + agent.destination);
            //agent.Stop();
            dest = new Vector3(newroom.transform.position.x, 1.2f, newroom.transform.position.z);
            //print("new:" + agent.destination);
            
        }
    
      
    }


  
}