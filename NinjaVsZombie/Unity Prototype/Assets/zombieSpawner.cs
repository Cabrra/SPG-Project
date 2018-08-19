using UnityEngine;
using System.Collections;

public class zombieSpawner : MonoBehaviour 
{

    public GameObject zombiespawn;
    public GameObject zombieinstance;
	// Use this for initialization
	void Start () 
    {
	
	}
	
	// Update is called once per frame
	void Update () 
    {
        GameObject temp = GameObject.FindGameObjectWithTag("Pizza");
        if (temp == null)
            return;
        float dist = Vector3.Distance(GameObject.FindGameObjectWithTag("Player").transform.position, temp.transform.position);
        if (dist > 10 && zombieinstance == null )
        {
            zombieinstance = (GameObject)Instantiate(zombiespawn, transform.position, transform.rotation);
        }
	}
}
