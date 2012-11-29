using System;
using UnityEngine;
using System.Collections;
using Random = UnityEngine.Random;

public class TimeManager : MonoBehaviour
{
    private string timeString;
    private static int timeHour;
    private static int timeMinutes;
    private float maxBrightness = 0.5f;

    public static bool LightsTurnOn;
    public GameObject DirectionLight;
    public GameObject SunRotator;

    private static int santaMoveMinute;
    private static int santaLastMoveHour = 59;

    private float sunStartLocation = 65f;
    private float sunStopLocation = -65f;
    private float sunFullDayRotation = 0;

	// Use this for initialization
	void Start ()
	{
	    LightsTurnOn = true;
	    GetRandomSantaTime();
	}
	
	// Update is called once per frame
	void Update ()
	{
        // Hour
        timeString = DateTime.Now.ToString("HH");
	    int.TryParse(timeString, out timeHour);

        // Seconds
        timeString = DateTime.Now.ToString("mm");
	    int.TryParse(timeString, out timeMinutes);

	    if (Input.GetKeyDown(KeyCode.Escape))
	        Application.Quit();

        // DEBUG TIME BUTTONS ---------------------------------
	    /*if (Input.GetKeyDown(KeyCode.H))
	    {
	        timeHour++;
	        if (timeHour >= 24)
	            timeHour = 0;
	    }

	    if (Input.GetKeyDown(KeyCode.M))
	    {
	        timeMinutes++;
	        if (timeMinutes > 60)
	        {
	            timeMinutes = 0;
	            timeHour++;
	        }
	    }*/

         //DEBUG TIME BUTTONS ---------------------------------

	    //print(timeHour + ":" + timeMinutes);

        //print("Time: " + timeHour + "Intensity: " + DirectionLight.light.intensity);

        // 0 - 0.5
        // 0 - 60

        // 10 - 12 = nat -> dag
        // 13-16 = fuld dag
        // 16-18 = nat


        switch (timeHour)
        {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                LightsTurnOn = true;
                DirectionLight.light.intensity = 0; // Hjoerring opens at 10
                break;
            case 10:
                LightsTurnOn = true;
                DirectionLight.light.intensity = 0; // Hjoerring opens at 10
                SunRotator.transform.rotation = Quaternion.Euler(0, 0, -16.25f * timeHour + 227.5f);

                break;

            case 11:                                                            // Becoming day
                DirectionLight.light.intensity = timeMinutes * (maxBrightness / 60);
                SunRotator.transform.rotation = Quaternion.Euler(0, 0,
                                                                 -16.25f*timeHour + 227.5f);

                
                if (timeMinutes == santaMoveMinute)
                {
                    SantaClaus.SantaMove = true;
                    santaLastMoveHour = 16;
                }
                break;
            case 12:
            case 13:
            case 14:
            case 15: // Full day 
                LightsTurnOn = false;
                DirectionLight.light.intensity = maxBrightness;

                if (timeMinutes == santaMoveMinute)
                {
                    SantaClaus.SantaMove = true;
                    santaLastMoveHour = 15;
                }
                
                SunRotator.transform.rotation = Quaternion.Euler(0, 0,
                                                                 -16.25f * timeHour + 227.5f);
                break;
            case 16:                                                             // Becoming night
                if (timeMinutes == santaMoveMinute)
                {
                    SantaClaus.SantaMove = true;
                    santaLastMoveHour = 16;
                }

                SunRotator.transform.rotation = Quaternion.Euler(0, 0,
                                                                 -16.25f * timeHour + 227.5f);

                break;
            case 17:                                                           
                if (timeMinutes == santaMoveMinute)
                {
                    SantaClaus.SantaMove = true;
                    santaLastMoveHour = 17;
                }
                SunRotator.transform.rotation = Quaternion.Euler(0, 0,
                                                                 -16.25f*timeHour + 227.5f);
                LightsTurnOn = true;
                DirectionLight.light.intensity = maxBrightness - (timeMinutes * (maxBrightness / 60));

                break;
            case 18: // Hjoering closes at 18                                   // Full night
                SunRotator.transform.rotation = Quaternion.Euler(0, 0,
                                                                 -16.25f*timeHour + 227.5f);
                DirectionLight.light.intensity = 0;

                //SunRotator.transform.rotation = Quaternion.Euler(0, 0, timeMinutes * (sunStopLocation / 60));


                break;
            case 19:
            case 20:
            case 21:
            case 22:
            case 23:
            case 24:
                DirectionLight.light.intensity = 0;
                SunRotator.transform.rotation = Quaternion.Euler(0, 0, sunStartLocation);
                break;
        }
	}

    public static void GetRandomSantaTime()
    {
        santaMoveMinute = Random.Range(1, 59);
        //print("Santa move minute: " + santaMoveMinute);
    }
}
