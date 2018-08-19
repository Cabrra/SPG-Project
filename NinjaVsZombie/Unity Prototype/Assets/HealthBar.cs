using UnityEngine;
using System.Collections;

public class HealthBar : MonoBehaviour {

	public float currHealth;
	public float maxHealth;
	public GUIStyle HealthStyle;

	// Use this for initialization
	void Start () 
	{

	}
	
	// Update is called once per frame
	void Update () {
		maxHealth = Player.Instance.maxhealth;
		currHealth = Player.Instance.health;


		if (Player.Instance.health > 0)
			currHealth = Player.Instance.health;
		else
			currHealth = 0;
//
//		if (currHealth == 0)
//			Application.LoadLevel(1);
	}

	void OnGUI() 
	{
		if (currHealth >= (maxHealth*2 / 3))
			ROG.DrawBar (currHealth / maxHealth, 110, 700, 190, 50,  Color.green,  currHealth.ToString() + '/' + maxHealth.ToString(), HealthStyle);
		else if (currHealth < (maxHealth*2 / 3) && currHealth > (maxHealth/3))
			ROG.DrawBar (currHealth / maxHealth, 110, 700, 190, 50, Color.yellow,  currHealth.ToString() + '/' + maxHealth.ToString(), HealthStyle);
		else if (currHealth > 0)
			ROG.DrawBar (currHealth / maxHealth, 110, 700, 190, 50, Color.red,  currHealth.ToString() + '/' + maxHealth.ToString(), HealthStyle);

	}
}
