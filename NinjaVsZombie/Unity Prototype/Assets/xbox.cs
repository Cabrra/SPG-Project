using UnityEngine;
using System.Collections;

public class xbox : MonoBehaviour 
 {
	public bool isActive = false;
		
		// Use this for initialization
		void Start () {
		guiTexture.enabled = isActive;
		}
		
		// Update is called once per frame
	void Update () 
	{
		if (Input.GetKeyDown (KeyCode.JoystickButton3)) 
		{
			isActive = !isActive;
			guiTexture.enabled = isActive;
		}
	}
}
