using UnityEngine;
using System.Collections;

public class LeverBehavior : MonoBehaviour {

	public bool isOn = false;
	public bool lvlmodifier = false;
	public int level = 1;
	public GameObject image;
	public GameObject indicator;
	public RoomTrigger doorToActivate;
	public RoomTrigger doorToActivate2;
	public AudioClip switchsnd;
	public int count;

	// Use this for initialization
	void Start () 
	{
		//isOn = false;
		count = 0;
	}

	// Update is called once per frame
	void Update () 
	{

		if(isOn == true && count <= 0)
		{
			image.renderer.material.mainTextureScale = new Vector2(-1,1);
			indicator.renderer.material.color = Color.green;
			count = 1;
			doorToActivate.unlocked = true;
			doorToActivate2.unlocked = true;
			doorToActivate.collider.isTrigger = true;
			doorToActivate2.collider.isTrigger = true;

			if(level < 5 && gameObject.tag == "Lever1")
				level = 5;
			if(level < 10 && gameObject.tag == "Lever2")
				level = 10;
		}
		else if(isOn == false)
		{
			image.renderer.material.mainTextureScale = new Vector2(1,1);
			indicator.renderer.material.color = Color.red;
			count = 0;
			doorToActivate.unlocked = false;
			doorToActivate2.unlocked = false;
			doorToActivate.collider.isTrigger = false;
			doorToActivate2.collider.isTrigger = false;
		}
	}

	void OnTriggerEnter(Collider other)
	{
		if(other.tag == "Hook" || other.tag == "Sword")
		{
			isOn = !isOn;
			if(switchsnd)
				ROG.PlaySound(switchsnd);
			GameObject.Find("Puzzle1").GetComponent<Puzzle1Behavior>().checkPuzzle(this.gameObject);
		}	
	}
}
