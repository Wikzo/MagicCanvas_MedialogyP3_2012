using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Text;
using UnityEngine;
using System.Collections;
using Random = UnityEngine.Random;

public class CharacterManager : MonoBehaviour
{
    private bool _fileExists;
    public float Character_Z_Default = -5f;
    public float Character_Y_Default = 100f;

    private float Snowman_Y_Min = 58.5f;
    private float Snowman_Y_Max = 75f;
    
    private float NisseGirl_Y_Min = 47f;
    private float NisseGirl_Y_Max = 60f;
    
    private float NisseBoy_Y_Min = 52.7f;
    private float NisseBoy_Y_Max = 81.3f;

    private float Angel_Y_Max = 155f;
    private float Angel_Y_Min = 123f;
    
    private float Squirrel_Y_Min = 50f;
    private float Squirrel_Y_Max = 71f;
    
    public GameObject[] CharacterPrefab;
    private int _characterNumber;

    public List<GameObject> Characters;
    private const float BACKGROUND_WIDTH = 220;
    private string _text;
    Vector3 _pixieDefaultPosition;
    private Vector3 outOfPicture_left = new Vector3(-32f, 90f, -5f);
    Vector3 outOfPicture_right = new Vector3(257f, 90f, -5f);
    private const float REFRESH_TIME = 0.5f;
    //  private float[] _xPositions;

    private string[] ID;
    private float[] Position;
    private Vector3[] target;

    private bool readyForNewTarget;

    //public int _numberOfPeopleRightNow;
    private float[] pointX;

    private static PropertyInfo _mSystemCopyBufferProperty = null;
    public float MoveSpeed = 2.6f;

    private float MovingThreshold = 2f;

    private float timer;
    private const float WaitTimeToGetNewTarget = 0.5f;

    private bool inputError;

    // Use this for initialization
	void Start ()
    {
        _pixieDefaultPosition = new Vector3(0, Character_Y_Default, Character_Z_Default);
	    _characterNumber = 0;

	    SpawnCharacters(10);
	    GetNewTarget();
        timer = WaitTimeToGetNewTarget;
	    inputError = false;

    }
	
	// Update is called once per frame
	void Update ()
	{
        GetReadyToMove();

        // Update target timer
	    if (timer < WaitTimeToGetNewTarget)
	    {
	        timer += Time.deltaTime;
	    }
	    else if (timer >= WaitTimeToGetNewTarget)
	    {
	        timer = 0;
	        GetNewTarget();
	    }

	}

    void SpawnCharacters(int count)
    {
        // new
        GetDataFromClipBoard();
        Characters = new List<GameObject>();
        for (int i = 0; i < count; i++)
        {
            GetRandomCharacter();
            GameObject temp = (GameObject)Instantiate(CharacterPrefab[_characterNumber], _pixieDefaultPosition, Quaternion.Euler(270f, 0, 0));

            if (i >= ID.Length)
                temp.name = "ID_waitingForInput_" + i;
            else
                temp.name = ID[i];

            Characters.Add(temp);
        }

        // new end


        #region OLD STUFF BEFORE NEW LOGGING SYSTEM

//// Spawn multiple
//        if (numberOfPixies > 1)
//        {
//            for (int i = 0; i < numberOfPixies; i++)
//            {
//                GetRandomCharacter();
//                GameObject p =
//                    (GameObject)
//                    Instantiate(CharacterPrefab[_characterNumber], _pixieDefaultPosition, Quaternion.Euler(270f, 0, 0));
//                p.name = "P" + i;
//                Characters.Add(p);
//            }
//        }
//        else if (numberOfPixies == 1)
//        {
//            GetRandomCharacter();
//            GameObject p =
//                (GameObject)
//                Instantiate(CharacterPrefab[_characterNumber], _pixieDefaultPosition, Quaternion.Euler(270f, 0, 0));
//            //p.name = "NewSpawner_" + Characters[Characters.Count - 1].name;
//            Characters.Add(p);
//        }

        #endregion

    }

    void GetRandomCharacter()
    {
        int randomCharacter = 0;
        int tryNumber = 0;

        // Try to get a character that has not yet been spawned
        while (tryNumber < 20 && _characterNumber == randomCharacter)
        {
            tryNumber++;
            randomCharacter = Random.Range(0, CharacterPrefab.Length);
        }

        _characterNumber = randomCharacter;
    }

    void OnGUI()
    {
        if (inputError)
            GUI.Label(new Rect(50, 50, 500, 200), "FEJL! - Der er noget galt med ClipBoard manageren.\n(Tryk Escape for at afslutte programmet)");
    }

    void GetDataFromClipBoard()
    {
        _text = ClipBoard; // raw data from clipboard

        // input should look like this:
        // pp0.1p0.2p0.3q

        // person 1 = 0.1
        // person 2 = 0.2
        // person 3 = 0.3

        // new

        // Check to see if OpenCV program is running and feeding us with the correct data
        if (!_text.StartsWith("q") && !_text.EndsWith("q"))
        {
            inputError = true;
            //throw new InvalidOperationException("ERROR - There is something wrong with the ClipBoard function data! Are you sure the OpenCV program is running?");
        }
        else
            inputError = true;

        // WE START WITH THIS DATA:
        // q i5p0.5 i42p0.5 i6p0.6 i7p0.341 i123p-1 q
        // [start character: q] [id tag: i] [person coordinate: p] [end character: q]
        string noWhiteSpace = _text.Replace(" ", string.Empty);
        string[] removeStartAndEnd = noWhiteSpace.Split('q'); // to get away with blank space

        // now we have: i5p0.5i6p0.6i7p0.7
        char[] c = new char[1];
        c[0] = 'i'; // used to split and not get empty string back
        string[] tempIDs = removeStartAndEnd[1].Split(c, StringSplitOptions.RemoveEmptyEntries);

        // now we have: [blankSpace];5p0.5;6p0.6;7p0.7
        ID = new string[tempIDs.Length];
        Position = new float[tempIDs.Length];

        // now we have: string array ID with names + float array Position with position values
        for (int i = 0; i < ID.Length; i++)
        {
            string[] temp = tempIDs[i].Split('p'); // split into IDs AND Positions
            ID[i] = "ID_" + temp[0]; // Save ID name
            float.TryParse(temp[1], out Position[i]); // Get position in float format

            if (Position[i] > 0f)
                Position[i] = 1f - Position[i]; // flip so it is not mirrored
        }
        //print("Here: " + Math.Round(Position[10]));
        //_numberOfPeopleRightNow = (int) ID.Length;
        
        // new end

        #region OLD STUFF BEFORE NEW LOGGING FORMAT

//        string[] lines = _text.Split('p'); // array with each BLOB data (separated by a 'p')
//        pointX = new float[lines.Length - 2]; // array with float points for each character
//
//        // Get first value - there is a mismatch between lines[] and pointX[], since lines[0] is EMPTY
//        for (int i = 0; i < pointX.Length; i++)
//        {
//            float.TryParse(lines[i + 1], out pointX[i]);
//        }
//
//        _numberOfPeopleRightNow = (int) pointX.Length; // first value is number of people

        #endregion


    }

    void GetNewTarget()
    {
        target = new Vector3[Position.Length];
        for (int i = 0; i < Characters.Count; i++)
        {
            if (Position[i] == -1) // don't give a target to an invalid character
                continue;

            if (Characters[i].tag == "Angel")
                target[i] = new Vector3(Position[i]*BACKGROUND_WIDTH, Random.Range(Angel_Y_Min, Angel_Y_Max),
                                        Character_Z_Default);
            else if (Characters[i].tag == "Snowman")
            {
                target[i] = new Vector3(Position[i]*BACKGROUND_WIDTH, Random.Range(Snowman_Y_Min, Snowman_Y_Max),
                                        Character_Z_Default);

                // Change moving sprite
                if (target[i].x > (Characters[i].transform.position.x + MovingThreshold)) // right
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingRight;
                else if (target[i].x < (Characters[i].transform.position.x - MovingThreshold)) // left
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingLeft;
                else // static
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.Static;
            }
            else if (Characters[i].tag == "Nisseboy")
            {
                target[i] = new Vector3(Position[i]*BACKGROUND_WIDTH, Random.Range(NisseBoy_Y_Min, NisseBoy_Y_Max),
                                        Character_Z_Default);

                // Change moving sprite
                if (target[i].x > (Characters[i].transform.position.x + MovingThreshold)) // right
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingRight;
                else if (target[i].x < (Characters[i].transform.position.x - MovingThreshold)) // left
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingLeft;
                else // static
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.Static;
            }
            else if (Characters[i].tag == "Nissegirl")
            {
                target[i] = new Vector3(Position[i]*BACKGROUND_WIDTH, Random.Range(NisseGirl_Y_Min, NisseGirl_Y_Max),
                                        Character_Z_Default);

                // Change moving sprite
                if (target[i].x > (Characters[i].transform.position.x + MovingThreshold)) // right
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingRight;
                else if (target[i].x < (Characters[i].transform.position.x - MovingThreshold)) // left
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingLeft;
                else // static
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.Static;
            }
            else if (Characters[i].tag == "Squirrel")
            {
                target[i] = new Vector3(Position[i]*BACKGROUND_WIDTH, Random.Range(Squirrel_Y_Min, Squirrel_Y_Max),
                                        Character_Z_Default);

                // Change moving sprite
                if (target[i].x > (Characters[i].transform.position.x + MovingThreshold)) // right
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingRight;
                else if (target[i].x < (Characters[i].transform.position.x - MovingThreshold)) // left
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingLeft;
                else // static
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.Static;

            }
        }
    }

    void SmoothMove()
    {
        for (int i = 0; i < Characters.Count; i++)
        {
            if (target[i].x > Characters[i].transform.position.x + MovingThreshold
                    || target[i].x < Characters[i].transform.position.x - MovingThreshold)
            {
                Vector3 direction = Vector3.Normalize(target[i] - Characters[i].transform.position);
                Characters[i].transform.position += direction*MoveSpeed*Time.deltaTime;
            }
        }
    }

    void GetReadyToMove()
    {
        GetDataFromClipBoard();

        if (Position.Length < Characters.Count)
        {
            inputError = true;
            //throw new InvalidOperationException("ERROR - input data is smaller than amount of characters in the list");
        }
        else if (Position.Length > Characters.Count)
        {
            inputError = true;
            //throw new InvalidOperationException("ERROR - input data is larger than amount of characters in the list");
        }
        else
            inputError = false;


        //print("POSITION LENGTH = " + Position.Length + " and Characters count = " + Characters.Count);
        //print("Actual pos: " + Characters[40].transform.position + " Wanted Pos: " + target[40]);

        // Update the pixie's positions
        for (int i = 0; i < Characters.Count; i++)
        {
            if (Position[i] == -1f)
            {
                //print("GETREADYTOMOVE");
                // if outside picture
                if (Characters[i].transform.position.x < outOfPicture_right.x &&
                    Characters[i].transform.position.x > outOfPicture_left.x)
                {
                    if (Characters[i].transform.position.x > 111)
                    {
                        GameObject oldCharacter = Characters[i];

                        oldCharacter.transform.position = outOfPicture_right;
                        GetRandomCharacter();
                        GameObject newCharacter = (GameObject)Instantiate(CharacterPrefab[_characterNumber], oldCharacter.transform.position,
                                        Quaternion.Euler(270f, 0, 0));

                        Characters[i] = newCharacter;

                        Destroy(oldCharacter);

                    }
                    else if (Characters[i].transform.position.x < 111)
                    {
                        GameObject oldCharacter = Characters[i];

                        oldCharacter.transform.position = outOfPicture_left;
                        GetRandomCharacter();
                        GameObject newCharacter = (GameObject)Instantiate(CharacterPrefab[_characterNumber], oldCharacter.transform.position,
                                        Quaternion.Euler(270f, 0, 0));

                        Characters[i] = newCharacter;

                        Destroy(oldCharacter);
                    }

                    Characters[i].name = ID[i] + "_invalid";
                }
            }
            else
            {
                Characters[i].name = ID[i];

                SmoothMove();

                //Characters[i].transform.position = new Vector3(Position[i]*BACKGROUND_WIDTH, Character_Y_Default,
                //Character_Z_Default);
            }
        }

        #region OLD STUFF BEFORE NEW LOGGING SYSTEM (kinda)

//        // Spawn more pixies if neccessary
//        if (_numberOfPeopleRightNow > Characters.Count)
//            SpawnCharacters(_numberOfPeopleRightNow - Characters.Count);
//        else if (_numberOfPeopleRightNow < Characters.Count)
//        {
//            GameObject p;
//            for (int i = 0; i < _numberOfPeopleRightNow; i++)
//            {
//                if (Position[i] <= -1f)
//                {
//                    p = GameObject.Find(ID[i]);
//                    Characters.RemoveAt(i);
//                    Destroy(p);
//                }
//            }
//
//            // TODO: Should delete the number that is removed and not just the last one in the list
//            p = Characters[Characters.Count - 1];
//            Characters.RemoveAt(Characters.Count - 1);
//            Destroy(p);
//
//
//
//        }
        #endregion

        
    }

    private static PropertyInfo GetSystemCopyBufferProperty()
    {
        if (_mSystemCopyBufferProperty == null)
        {
            Type T = typeof(GUIUtility);
            _mSystemCopyBufferProperty = T.GetProperty("systemCopyBuffer", BindingFlags.Static | BindingFlags.NonPublic);
            if (_mSystemCopyBufferProperty == null)
                throw new Exception("Can't access internal member 'GUIUtility.systemCopyBuffer' it may have been removed / renamed");
        }
        return _mSystemCopyBufferProperty;
    }

    public static string ClipBoard
    {
        get
        {
            PropertyInfo P = GetSystemCopyBufferProperty();
            return (string)P.GetValue(null, null);
        }
        set
        {
            PropertyInfo P = GetSystemCopyBufferProperty();
            P.SetValue(null, value, null);
        }
    }

    // OLD STUFF BEGIN  ----------------------------------------------------
    /*
    void GetPositionFromTextFile()
    {
        /*
	    if (!File.Exists("test.txt"))
	    {
	        print("FILES DOES NOT EXIST!");
	        fileExists = false;
	    }
	    else
	    {
            print("FILES DOES INDEED EXIST!");
            fileExists = true;
	    }*/

        // Used for reading txt files ... WE DON'T USE THIS ANYMORE
        /*using (StreamReader reader = new StreamReader("test.txt"))
        {
            string line;

            if (reader.ReadLine() != null)
            {
                line = reader.ReadLine();

                string[] lines = line.Split('p');
                
                foreach (string l in lines)
                {
                    Characters.Add(CharacterPrefab[0]);
                }

                float[] pointX = new float[lines.Length];

                for (int i = 0; i < lines.Length; i++)
                {
                    float.TryParse(lines[i], out pointX[i]);
                }
                //int.TryParse(lines[0], out pointX);
                for (int i = 0; i < pointX.Length; i++)
                {
                    print(pointX[i]);
                    Characters[i].transform.position = new Vector3(pointX[i], Character_Y_Default, Character_Z_Default);
                }
               
                //int.TryParse(lines[1], out pointY);

                //print("X: " + pointX + ", Y: " + pointY);

                //return new Vector3(pointX, Character_Y_Default, Character_Z_Default);


            }
            else
            {
                print("ERROR - couldn't get move data from clipboard");
            }
        }
    }

    IEnumerator UpdatePosition()
    {
        oldMovePixies();
        yield return new WaitForSeconds(REFRESH_TIME);
        //StartCoroutine(UpdatePosition());
    }

    void KillPixies(bool removeAllPixies)
    {
        // Remove all
        if (removeAllPixies)
        {
            foreach (GameObject pixie in Characters)
            {
                Destroy(pixie);
            }
            Characters.Clear();
        }
            // Remove the last pixie
        else
        {
            GameObject p = Characters[Characters.Count - 1];
            Characters.RemoveAt((Characters.Count - 1));
            Destroy(p);
        }
    }

    void oldMovePixies()
    {
        for (int i = 0; i < Characters.Count - 1; i++)
        {
            //print(posX[i]);
            //Characters[i].transform.position = new Vector3(_xPositions[i] * BACKGROUND_WIDTH, Character_Y_Default, Character_Z_Default);
            /*Characters[i].transform.position = Vector3.Lerp(transform.position, new Vector3(_xPositions[i] * BACKGROUND_WIDTH,
                                                                                    transform.position.y,
                                                                                    transform.position.y),
                                                    Time.deltaTime * REFRESH_TIME);
            //print(Characters[i].name + ". " + Characters[i].transform.position);

        }
    }

    int GetNumberOfPlayers()
    {
        _text = ClipBoard; // raw data from clipboard

        string[] lines = _text.Split('n'); // number of players

        int number;
        int.TryParse(lines[0], out number);

        return number;

    }

    void GetXPositionFromClipBoard_FirstTime()
    {
        _text = ClipBoard; // raw data from clipboard

        string[] lines = _text.Split('p'); // array with each BLOB data (separated by a 'p')

        float floatNumber;
        bool isValidNumber = float.TryParse(lines[0], out floatNumber);

        if (!isValidNumber)
            throw new InvalidOperationException("ERROR - There is something wrong with the ClipBoard function data! Are you sure the OpenCV program is running?");
        
        _numberOfPeopleRightNow = lines.Length - 1;
    }

    void UpdateNumberOfPlayers()
    {
        int newSize = GetNumberOfPlayers();

        // Spawn 1 new pixie
        if (Characters.Count - 1 < newSize)
        {
            //SpawnCharacters();
        }
        // Remove 1 pixie
        else
        {
            KillPixies(false);
        }
    }

    // OLD STUFF END ------------------------------------------------------

    */

}
