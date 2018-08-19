using UnityEngine;
using System.Collections;

public class Player : MonoBehaviour {

    public static Player Instance;
    public int MoveSpeed = 5;
	public float gravity = 20;

    public GameObject Hook;
    private GameObject HookInstance;
    public GameObject Pizza;
    public MeleeWeapon Sword;

    public int score = 0; //Currency
    private CharacterController controller;

    public Vector3 move = Vector3.zero;
    public Vector3 rotation = Vector3.zero;

    private float nextShootTime = 0;
    public int health = 100;
	public int maxhealth = 100;
    public bool havePizza = true;
    public bool CamMove = false;
    public bool ishookEquiped, isSwordEquiped;
	public bool isAttacked = false;
	
	public int countbeat = 5;
	public int currentitem = 0;
	public int level = 1;
	public bool fight = false;

	public AudioClip Hurt;
	public AudioClip Katana;
	public AudioClip hookSound;
	public AudioClip scoreUp;
	public AudioClip DropPizza;
	public AudioClip TakePizza;
	public AudioClip ItemSwitch;
	public AudioClip LevelUp;
	
	// Use this for initialization
	void Start () 
    {
		currentitem = 1;
        Instance = this;
        controller = this.GetComponent<CharacterController>();
        ishookEquiped = true;
        isSwordEquiped = false;
	}
	
	// Update is called once per frame
	void Update () 
    {

		if (isAttacked == true) 
		{
			Attacking();
		}
        if (!isAttacked && PauseScreen.Instance.isPaused == false)
        {
			countbeat = 5;
			Vector3 inputVector = Vector3.zero;
			//inputVector.x = Input.GetAxis("Horizontal");
			//inputVector.z = Input.GetAxis("Vertical");
			if (Input.GetKey(KeyCode.W)||Input.GetAxis("Vertical")>.5)
				inputVector.z = 1;
			else if (Input.GetKey(KeyCode.S)||Input.GetAxis("Vertical")<-.5) 
				inputVector.z = -1;
			else
				inputVector.z = 0;
			
			if (Input.GetKey(KeyCode.A)||Input.GetAxis("Horizontal")<-.5)
				inputVector.x = -1;
			else if (Input.GetKey(KeyCode.D)||Input.GetAxis("Horizontal")>.5) 
				inputVector.x = 1;
			else
				inputVector.x = 0;

			move.x = (inputVector.x) * MoveSpeed * level;
			move.z = (inputVector.z) * MoveSpeed * level;

            if (!HookInstance)
            {
				if(Input.GetKeyDown(KeyCode.K)||Input.GetKeyDown(KeyCode.JoystickButton3))
                {
					if (ItemSwitch && havePizza == false)
						ROG.PlaySound(ItemSwitch);
                    if(ishookEquiped == true)
                    {
                        ishookEquiped = false;
                        isSwordEquiped = true;
						if (havePizza == false)
							currentitem = 2;
                    }
                    else if (isSwordEquiped == true)
                    {
                        ishookEquiped = true;
                        isSwordEquiped = false;
						if (havePizza == false)
							currentitem= 1;
                    }
                }
				rotation = move;
				rotation.y = 0;
				if (Mathf.Round( move.x) > 0 || Mathf.Round(move.x) < 0) 
					rotation.z = 0;

				
				if(rotation.magnitude >= 1.0f)
					transform.rotation = Quaternion.LookRotation(rotation);

                controller.Move(move * Time.deltaTime);

                
                //transform.rotation = Quaternion.LookRotation(move);

				if((Input.GetKeyDown(KeyCode.J) || Input.GetKeyDown(KeyCode.JoystickButton2))&& !havePizza && isSwordEquiped)
				{
					if (Katana)
						ROG.PlaySound (Katana);
                    Sword.Swing();
				}
            }
            rotation.y = 0;
			if ((Input.GetKeyDown(KeyCode.J)||Input.GetKeyDown(KeyCode.JoystickButton2)) && Time.time > nextShootTime && !havePizza && ishookEquiped)
			{
				if(hookSound)
					ROG.PlaySound(hookSound);
				Shoot();
			}
			if(!controller.isGrounded)
			{
				if (CamControl.Instance.transform.position.x != -40 || (CamControl.Instance.transform.position.x == -40 && Player.Instance.transform.position.x > -26.5f))
					move.y -= gravity * Time.deltaTime;
				if (CamControl.Instance.transform.position.x == -40 && move.y <= -1)
				{
					if (Player.Instance.transform.position.x > -26.5f)
					{
						if (Player.Instance.transform.position.y < -1 && Player.Instance.transform.position.y >= -14)
							Player.Instance.transform.localScale *= 0.95f; //= new Vector3(0.5f , 1 ,0.5f);
						else if (Player.Instance.transform.position.y < -15)
						{
							Player.Instance.transform.position = new Vector3 ( -40.0f, 2.0f, -10.0f);
							Player.Instance.transform.localScale = new Vector3(1.0f, 1.0f ,1.0f);
							ModifyHealth(-10);
						}
					}
//					else // fire
//					{
//						if (Player.Instance.transform.position.y < -0.1)
//						{
//							ModifyHealth(-5);
//							Player.Instance.transform.position = new Vector3 ( -40.0f, 2.0f, -10.0f);
//						}
//					}
				}
				if (CamControl.Instance.transform.position.x == 0 && move.y <= -1)
				{
					if (Player.Instance.transform.position.y < -1 && Player.Instance.transform.position.y >= -28)
						Player.Instance.transform.localScale *= 0.95f; //= new Vector3(0.5f , 1 ,0.5f);
					else if (Player.Instance.transform.position.y < -30)
					{
						Player.Instance.transform.position = new Vector3( -11.0f, 2.0f, 3.0f);
						Player.Instance.ModifyHealth (-10);	
						Player.Instance.transform.localScale = new Vector3(1.0f, 1.0f ,1.0f);
					}
				}
			}
			if ((Input.GetKeyDown(KeyCode.Space)||Input.GetKeyDown(KeyCode.JoystickButton0)) && havePizza )
            {
                if (Pizza)
				{
					if(DropPizza)
						ROG.PlaySound(DropPizza);
                    Instantiate(Pizza, transform.position + transform.forward * 2, transform.rotation);
					if(isSwordEquiped)
					currentitem=2;
					else
					currentitem=1;					
				}
					
				havePizza = false;
            }
        }
	}

	void Attacking()
	{
		if ((Input.GetKeyDown (KeyCode.O) || Input.GetKeyDown(KeyCode.JoystickButton1)) && Player.Instance.havePizza) 
		{
			fight = true;
			countbeat--;
		}
	}
    //-------------------- Shoot -----------------------    
    void Shoot()
    {
        if(Hook && !HookInstance)
           HookInstance = (GameObject)Instantiate(Hook, transform.position + transform.forward * 1, transform.rotation);
        
        nextShootTime = Time.time + .2f;
    }

    void ModifyHealth(int amount)
    {
		if (Hurt)
			ROG.PlaySound (Hurt);
        health += amount;
        if (health < 1)
			Application.LoadLevel(7);
    }

	void ModifyScore(int amount)
	{
//		if (scoreUp)
//			ROG.PlaySound (scoreUp);
//		score += amount;
	}
	
	void HavePizza(bool Have)
    {
		if(TakePizza)
			ROG.PlaySound(TakePizza);
        havePizza = Have;
    }
}