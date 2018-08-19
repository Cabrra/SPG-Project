using UnityEngine;
using System.Collections;

public class RoomTrigger : MonoBehaviour {
	public float x = 0;
	public float z = 0;
	public bool unlocked = true;

	// Use this for initialization
	void Start () 
	{

	}
	
	// Update is called once per frame
	void Update () 
	{
		if(unlocked)
			renderer.material.color = Color.green;
		else
			renderer.material.color = Color.red;
	}

	void OnTriggerEnter(Collider other)
	{
		if(other.tag == "Player")
		{
			if(unlocked == true)
			{
				CamControl c = Camera.main.GetComponent<CamControl>();
				c.camPos = new Vector3(x,20,z);
				other.SendMessage("RoomChange",SendMessageOptions.DontRequireReceiver);
				renderer.material.color = Color.green;
				this.collider.isTrigger = true;
			}
			else
			{
				this.collider.isTrigger = false;
			}
		}
	}

}
