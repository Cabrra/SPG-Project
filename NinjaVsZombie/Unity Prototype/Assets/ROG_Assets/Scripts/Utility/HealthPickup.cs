using UnityEngine;
using System.Collections;

public class HealthPickup : MonoBehaviour 
{
	public float healthAmount = 50;

	void OnTriggerEnter (Collider other) 
	{		
		other.SendMessage("ModifyHealth", healthAmount, SendMessageOptions.DontRequireReceiver);
		Destroy(this.gameObject);
	}
}
