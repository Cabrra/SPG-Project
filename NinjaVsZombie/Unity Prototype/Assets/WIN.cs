using UnityEngine;
using System.Collections;

public class WIN : MonoBehaviour {


	float timer = 2.75f;
	// Use this for initialization
	void Start () {
		guiTexture.enabled = false;
	}
	
	// Update is called once per frame
	void Update () 
	{
		if (timer > 0)
			timer -= Time.deltaTime;		
		if (timer < 0) 
		{
			guiTexture.enabled = true;
		}
	}
}
