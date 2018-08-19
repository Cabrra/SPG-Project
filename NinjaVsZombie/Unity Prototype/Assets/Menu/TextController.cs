using UnityEngine;
using System.Collections;

public class TextController : MonoBehaviour {
	
	public bool isQuit = false;
	public Color OnMouseOverColor = Color.white;
	public Color OnMouseExitColor;

	void Start () 
	{
		OnMouseExitColor = GetComponent<TextMesh> ().color;
	}

	void Update()
	{
		if (Input.GetKeyDown(KeyCode.Return)|| Input.GetKeyDown(KeyCode.JoystickButton0))
		    Application.LoadLevel (3);
		else  if (Input.GetKeyDown(KeyCode.Escape) || Input.GetKeyDown(KeyCode.JoystickButton1))
		    Application.Quit();
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
