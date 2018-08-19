using UnityEngine;
using System.Collections;

public class PizzaBehaviour : MonoBehaviour {



	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}
    // Collision event
    void OnTriggerEnter(Collider other)
    {
        if (other.tag == "Player" || other.tag == "Hook")
        {

            // Destroy the bullet
            Destroy(gameObject);
        
            // Send a message to the other object (call HavePizza() if it exists)
            other.SendMessage("HavePizza", true, SendMessageOptions.DontRequireReceiver);
			Player.Instance.currentitem = 0;
        }
    }
}
