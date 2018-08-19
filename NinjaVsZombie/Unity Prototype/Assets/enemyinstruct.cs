using UnityEngine;
using System.Collections;

public class enemyinstruct : MonoBehaviour {

	public Color OnMouseOverColor = Color.white;
	public Color OnMouseExitColor;
	public bool isExit = false;
	
	// Use this for initialization
	void Start () 
	{
		OnMouseExitColor = GetComponent<TextMesh> ().color;
	}
	
	void OnMouseUp()
	{
		if (isExit == true)
			Application.LoadLevel(2); //main menu
		else
			Application.LoadLevel(5); //page 1
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