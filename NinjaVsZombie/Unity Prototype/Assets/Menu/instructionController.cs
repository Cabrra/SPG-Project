using UnityEngine;
using System.Collections;

public class instructionController : MonoBehaviour
	{

	public Color OnMouseOverColor = Color.white;
	public Color OnMouseExitColor;
	//public bool isExit = false;

	// Use this for initialization
	void Start () 
	{
		OnMouseExitColor = GetComponent<TextMesh> ().color;
	}
	
	void OnMouseUp()
	{
//		if (isExit == true)
//			Application.LoadLevel(1); //main menu
//		else
			Application.LoadLevel(4); //page 2
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