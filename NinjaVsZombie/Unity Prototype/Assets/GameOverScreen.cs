using UnityEngine;
using System.Collections;

public class GameOverScreen : MonoBehaviour {
	
	public bool isExit = false;
	
	// Use this for initialization
	void Start () 
	{
	
	}
	
	void Update()
	{
		if (Input.GetKeyDown(KeyCode.W) || Input.GetKeyDown(KeyCode.JoystickButton0) )
			Application.LoadLevel(2);
		if (Input.GetKeyDown(KeyCode.S) || Input.GetKeyDown(KeyCode.JoystickButton3) )
			Application.LoadLevel(1);
	}
}