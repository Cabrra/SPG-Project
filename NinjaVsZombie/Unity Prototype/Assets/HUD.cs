using UnityEngine;
using System.Collections;

public class HUD : MonoBehaviour {

	public Texture2D[] items;
	public int currItem = 0;

	// Use this for initialization
	void Start () 
	{
	

	}
	
	// Update is called once per frame
	void OnGUI () {



		currItem = Player.Instance.currentitem;


		Rect r = new Rect (Screen.width - 210, Screen.height - 75, 64, 64);
		for (int i =0; i<items.Length; i++) 
		{
			if (i == 0 && Player.Instance.havePizza == false)
				continue;
			GUI.DrawTexture(r, items[i]);
			if (currItem == i)
				ROG.DrawBoxOutline (r, 4, Color.blue, 1.0f);
			r.x += 65;	

		}



	
	}
}
