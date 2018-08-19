using UnityEngine;
using System.Collections;

public class Instruct1 : MonoBehaviour {
	
	public Color OnMouseOverColor = Color.white;
	public Color OnMouseExitColor;
	public bool isExit = false;
	
	// Use this for initialization
	void Start () 
	{
		OnMouseExitColor = GetComponent<TextMesh> ().color;
	}

	void Update()
	{
		if (Input.GetKeyDown(KeyCode.Return) || Input.GetKeyDown(KeyCode.JoystickButton0))
			Application.LoadLevel(4);
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