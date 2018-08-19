using UnityEngine;
using System.Collections;

public class score_GUI : MonoBehaviour {
	
	// Use this for initialization
	void Start () {

	}
	
	// Update is called once per frame
	void Update () 
	{
		//int score = 0;
		this.guiText.text = Player.Instance.score.ToString();
	}
}
