using UnityEngine;
using System.Collections;

public class instructionController2 : MonoBehaviour {

	public Color OnMouseOverColor = Color.white;
	public Color OnMouseExitColor;
	public bool isExit = false;
	public float timer;
	
	// Use this for initialization
	void Start () 
	{
		timer = 1.0f;
		OnMouseExitColor = GetComponent<TextMesh> ().color;
	}

	void Update()
	{
		if (timer >= 0)
			timer -= Time.deltaTime;
		else {
			if (Input.GetKeyDown (KeyCode.Escape) || Input.GetKeyDown (KeyCode.JoystickButton1))
				Application.LoadLevel (3);
			else if (Input.GetKeyDown (KeyCode.Return) || Input.GetKeyDown (KeyCode.JoystickButton0))
				Application.LoadLevel (2);
		}
	}

	void OnMouseOver()
	{
		GetComponent<TextMesh>().color = OnMouseOverColor;
	}
	void OnMouseExit()
	{
		GetComponent<TextMesh>().color = OnMouseExitColor;
	}
}