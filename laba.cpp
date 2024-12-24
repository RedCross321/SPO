#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <algorithm>

using namespace std;

class Film;
class Series;

class MediaContent {
protected:
    string name;
    string country;
    string director;
    int year;

public:
    MediaContent() : name(""), country(""), director(""), year(0) {}
    
    virtual ~MediaContent() = default;

    virtual void print() const = 0;
    virtual string getType() const = 0;
    
    virtual void readSpecificData(ifstream& file) = 0;
    virtual void writeSpecificData(ofstream& file) const = 0;
    virtual void inputSpecificData() = 0;
    virtual void setSpecificData(const string& data) = 0;

    string getName() const { return name; }
    string getCountry() const { return country; }
    string getDirector() const { return director; }
    int getYear() const { return year; }

    void setName(const string& n) { name = n; }
    void setCountry(const string& c) { country = c; }
    void setDirector(const string& d) { director = d; }
    void setYear(int y) { year = y; }
};

class Film : public MediaContent {
private:
    string actor;
    int duration;

public:
    Film() : MediaContent(), actor(""), duration(0) {}
    
    Film(const Film& other) 
        : MediaContent(other), actor(other.actor), duration(other.duration) {}

    Film(const Film& original, const string& newDirector) {
        name = original.name + " (Director's Cut)";
        country = original.country;
        director = newDirector;
        actor = original.actor;
        year = original.year;
        duration = original.duration;
    }

    string getActor() const { return actor; }
    void setActor(const string& a) { actor = a; }

    int getDuration() const { return duration; }
    void setDuration(int d) { duration = d; }

    string getType() const override { return "Film"; }

    void readSpecificData(ifstream& file) override {
        getline(file, actor);
    }

    void writeSpecificData(ofstream& file) const override {
        file << actor << "\n";
    }

    void inputSpecificData() override {
        cout << "\x1B[5;32mMain actor: \x1b[0m";
        getline(cin, actor);
    }

    void setSpecificData(const string& data) override {
        actor = data;
    }

    void print() const override {
        cout << "\x1B[0;32;40m" 
             << "[Film] "
             << name << " | " 
             << country << " | "
             << director << " | "
             << actor << " | "
             << year 
             << "\x1b[0m\n";
    }

    Film operator+(const Film& other) const {
        Film result;
        result.name = this->name + " & " + other.name;
        result.country = this->country + "/" + other.country;
        result.director = this->director + " & " + other.director;
        result.actor = this->actor + " & " + other.actor;
        result.year = max(this->year, other.year) + 1;
        result.duration = this->duration + other.duration;
        return result;
    }

    Film operator*(int count) const {
        Film result;
        result.name = this->name + " x" + to_string(count);
        result.country = this->country;
        result.director = this->director;
        result.actor = this->actor;
        result.year = this->year + (2 * count);
        result.duration = this->duration * count;
        return result;
    }

    Film& operator++() {
        ++year;
        return *this;
    }

    Film operator++(int) {
        Film temp = *this;
        temp.name += " 2";
        temp.year++;
        return temp;
    }

    bool operator==(const Film& other) const {
        return (year == other.year) && (name == other.name);
    }

    bool operator!=(const Film& other) const {
        return !(*this == other);
    }

    static void clear() {
        system("cls");
    }
};

class Series : public MediaContent {
protected:
    int episodes;
    int seasons;
    int episodeDuration;

public:
    friend class FilmDatabase;

    Series() : MediaContent(), episodes(0), seasons(0), episodeDuration(0) {}
    
    void setEpisodes(int e) { episodes = e; }
    void setSeasons(int s) { seasons = s; }
    void setEpisodeDuration(int d) { episodeDuration = d; }
    int getEpisodes() const { return episodes; }
    int getSeasons() const { return seasons; }
    int getEpisodeDuration() const { return episodeDuration; }

    string getType() const override { return "Series"; }

    void readSpecificData(ifstream& file) override {
        file >> seasons >> episodes >> episodeDuration;
        file.ignore();
    }

    void writeSpecificData(ofstream& file) const override {
        file << seasons << " " << episodes << " " << episodeDuration << "\n";
    }

    void inputSpecificData() override {
        cout << "\x1B[5;32mSeasons: \x1b[0m";
        cin >> seasons;
        cin.ignore();
        cout << "\x1B[5;32mEpisodes: \x1b[0m";
        cin >> episodes;
        cin.ignore();
        cout << "\x1B[5;32mEpisode Duration (minutes): \x1b[0m";
        cin >> episodeDuration;
        cin.ignore();
    }

    void setSpecificData(const string& data) override {
        size_t space1 = data.find(' ');
        size_t space2 = data.find(' ', space1 + 1);
        if (space1 != string::npos && space2 != string::npos) {
            seasons = stoi(data.substr(0, space1));
            episodes = stoi(data.substr(space1 + 1, space2 - space1 - 1));
            episodeDuration = stoi(data.substr(space2 + 1));
        }
    }

    void print() const override {
        cout << "\x1B[0;32;40m" 
             << "[Series] "
             << name << " | " 
             << country << " | "
             << director << " | "
             << seasons << " seasons | "
             << episodes << " episodes | "
             << episodeDuration << " min/ep | "
             << "Total: " << (seasons * episodes * episodeDuration) << " minutes | "
             << year 
             << "\x1b[0m\n";
    }
};

class MiniSeries : public Series {
private:
    bool limitedRun;
public:
    MiniSeries() : Series(), limitedRun(true) {
        seasons = 1;
    }

    void setLimitedRun(bool limited) { limitedRun = limited; }
    bool isLimitedRun() const { return limitedRun; }

    string getType() const override { return "MiniSeries"; }

    void readSpecificData(ifstream& file) override {
        file >> episodes;
        file.ignore();
        file >> limitedRun;
        file.ignore();
    }

    void writeSpecificData(ofstream& file) const override {
        file << episodes << "\n" << limitedRun << "\n";
    }

    void inputSpecificData() override {
        cout << "\x1B[5;32mEpisodes: \x1b[0m";
        cin >> episodes;
        cin.ignore();
        cout << "\x1B[5;32mLimited Run (1/0): \x1b[0m";
        cin >> limitedRun;
        cin.ignore();
    }

    void setSpecificData(const string& data) override {
        episodes = stoi(data);
    }

    void print() const override {
        cout << "\x1B[0;32;40m" 
             << "[MiniSeries] "
             << name << " | " 
             << country << " | "
             << director << " | "
             << episodes << " episodes | "
             << (limitedRun ? "Limited Run" : "Ongoing") << " | "
             << year 
             << "\x1b[0m\n";
    }
};

class FilmDatabase {
private:
    MediaContent** films;
    size_t size;
    size_t capacity;

    friend bool validateDatabaseSize(const FilmDatabase& db);

public:
    FilmDatabase() : films(nullptr), size(0), capacity(0) {}
    
    ~FilmDatabase() {
        for (size_t i = 0; i < size; ++i) {
            delete films[i];
        }
        delete[] films;
    }

    void addFilm() {
        MediaContent* film;
        string temp;

        Film::clear();
        cout << "\x1B[5;32mName: \x1b[0m";
        string name;
        getline(cin, name);

        cout << "\x1B[5;32mCountry: \x1b[0m";
        string country;
        getline(cin, country);

        cout << "\x1B[5;32mProducer: \x1b[0m";
        string director;
        getline(cin, director);

        cout << "\x1B[5;32mType (Film/f, Series/s, MiniSeries/ms): \x1b[0m";
        string type;
        getline(cin, type);
        
        string lowerType = type;
        transform(lowerType.begin(), lowerType.end(), lowerType.begin(), ::tolower);

        if (type == "film" || lowerType == "f") {
            film = new Film();
            film->inputSpecificData();
        } else if (type == "series" || lowerType == "s") {
            film = new Series();
            film->inputSpecificData();
        } else if (type == "miniseries" || lowerType == "ms") {
            film = new MiniSeries();
            film->inputSpecificData();
        } else {
            cout << "\x1B[0;32mInvalid type\x1b[0m\n";
            return;
        }

        film->setName(name);
        film->setCountry(country);
        film->setDirector(director);

        cout << "\x1B[5;32mYear: \x1b[0m";
        int year;
        cin >> year;
        film->setYear(year);
        cin.ignore();

        if (size >= capacity) {
            resize(capacity * 2);
        }
        films[size++] = film;

        Film::clear();
        cout << "\x1B[0;32mFilm was add\x1b[0m\n";
        cout << "\x1B[5;32mWant to add more? (y/n): \x1b[0m";
        char answer;
        cin >> answer;
        cin.ignore();
        if (answer == 'y' || answer == 'Y') {
            addFilm();
        }
    }

    void deleteFilm() {
        Film::clear();
        int index;
        cout << "\x1B[0;32mEnter a number of film what you want delete: \x1b[0m";
        cin >> index;
        cin.ignore();

        if (index <= 0 || index > size) {
            Film::clear();
            cout << "\x1B[0;32mFilm with this number don't exists\x1b[0m\n";
        } else {
            delete films[index - 1];
            for (size_t i = index - 1; i < size - 1; ++i) {
                films[i] = films[i + 1];
            }
            --size;
            Film::clear();
            cout << "\x1B[0;32mFilm was succesfully delete\x1b[0m\n";
        }
    }

    void printFilms() const {
        Film::clear();
        cout << "\x1B[0;32;40m  № | Name                     | Country             | Producer                    | Type                      |Year  \x1b[0m\n";
        cout << "\x1B[0;32;40m-------------------------------------------------------------------------------------------------------------------------- \x1b[0m\n";
        for (size_t i = 0; i < size; ++i) {
            cout << "\x1B[0;32;40m" << i + 1 << "|\x1b[0m";
            films[i]->print();
        }
    }

    void searchFilmByActor() const {
        Film::clear();
        cout << "\x1B[0;32mEnter a name of actor: \x1b[0m";
        string actor;
        getline(cin, actor);

        Film::clear();
        cout << "\x1B[0;32;40m  № | Name                     | Country             | Producer                    | Type                      |Year  \x1b[0m\n";
        cout << "\x1B[0;32;40m-------------------------------------------------------------------------------------------------------------------------- \x1b[0m\n";

        bool found = false;
        for (size_t i = 0; i < size; ++i) {
            if (films[i]->getType() == "Film") {
                Film* film = dynamic_cast<Film*>(films[i]);
                if (film->getActor() == actor) {
                    cout << "\x1B[0;32;40m" << i + 1 << "|\x1b[0m";
                    films[i]->print();
                    found = true;
                }
            }
        }

        if (!found) {
            Film::clear();
            cout << "\x1B[0;32mFilm with this name of actor don't exists.\x1b[0m\n";
        }
    }

    size_t getFilmCount() const {
        return size;
    }

    const MediaContent* const* getFilms() const {
        return films;
    }

    size_t getSize() const {
        return size;
    }

    void setFilms(MediaContent** newFilms, size_t newSize) {
        for (size_t i = 0; i < size; ++i) {
            delete films[i];
        }
        delete[] films;
        films = new MediaContent*[newSize * 2];
        size = newSize;
        capacity = newSize * 2;
        for (size_t i = 0; i < size; ++i) {
            films[i] = newFilms[i];
        }
    }

    void combineFilms() {
        if (size < 2) {
            cout << "\x1B[0;32mNeed at least 2 films to combine\x1b[0m\n";
            return;
        }

        cout << "Enter first film number: ";
        int idx1;
        cin >> idx1;
        cout << "Enter second film number: ";
        int idx2;
        cin >> idx2;
        cin.ignore();

        if (idx1 <= 0 || idx2 <= 0 || idx1 > size || idx2 > size) {
            cout << "\x1B[0;32mInvalid film numbers\x1b[0m\n";
            return;
        }

        if (films[idx1-1]->getType() == "Film" && films[idx2-1]->getType() == "Film") {
            Film* film1 = dynamic_cast<Film*>(films[idx1-1]);
            Film* film2 = dynamic_cast<Film*>(films[idx2-1]);
            Film combined = *film1 + *film2;
            if (size >= capacity) {
                resize(capacity * 2);
            }
            films[size++] = new Film(combined);
            cout << "\x1B[0;32mFilms combined successfully\x1b[0m\n";
        } else {
            cout << "\x1B[0;32mOnly films can be combined\x1b[0m\n";
        }
    }

    void multiplyFilm() {
        if (size < 1) {
            cout << "\x1B[0;32mNo films to multiply\x1b[0m\n";
            return;
        }

        cout << "Enter film number: ";
        int idx;
        cin >> idx;
        cout << "Enter multiplication factor: ";
        int factor;
        cin >> factor;
        cin.ignore();

        if (idx <= 0 || idx > size || factor <= 0) {
            cout << "\x1B[0;32mInvalid input\x1b[0m\n";
            return;
        }

        if (films[idx-1]->getType() == "Film") {
            Film* film = dynamic_cast<Film*>(films[idx-1]);
            Film multiplied = *film * factor;
            if (size >= capacity) {
                resize(capacity * 2);
            }
            films[size++] = new Film(multiplied);
            cout << "\x1B[0;32mFilm multiplied successfully\x1b[0m\n";
        } else {
            cout << "\x1B[0;32mOnly films can be multiplied\x1b[0m\n";
        }
    }

    void incrementFilm() {
        if (size < 1) {
            cout << "\x1B[0;32mNo films to increment\x1b[0m\n";
            return;
        }

        cout << "Enter film number: ";
        int idx;
        cin >> idx;
        cin.ignore();

        if (idx <= 0 || idx > size) {
            cout << "\x1B[0;32mInvalid film number\x1b[0m\n";
            return;
        }

        if (films[idx-1]->getType() == "Film") {
            Film* film = dynamic_cast<Film*>(films[idx-1]);
            cout << "Choose increment type:\n";
            cout << "1. Prefix (++film) - increase year\n";
            cout << "2. Postfix (film++) - create sequel\n";
            int choice;
            cin >> choice;
            cin.ignore();

            if (choice == 1) {
                ++(*film);
                cout << "\x1B[0;32mFilm year increased\x1b[0m\n";
            } else if (choice == 2) {
                if (size >= capacity) {
                    resize(capacity * 2);
                }
                films[size++] = new Film((*film)++);
                cout << "\x1B[0;32mSequel created\x1b[0m\n";
            } else {
                cout << "\x1B[0;32mInvalid choice\x1b[0m\n";
            }
        } else {
            cout << "\x1B[0;32mOnly films can be incremented\x1b[0m\n";
        }
    }

    void compareFilms() {
        if (size < 2) {
            cout << "\x1B[0;32mNeed at least 2 films to compare\x1b[0m\n";
            return;
        }

        cout << "Enter first film number: ";
        int idx1;
        cin >> idx1;
        cout << "Enter second film number: ";
        int idx2;
        cin >> idx2;
        cin.ignore();

        if (idx1 <= 0 || idx2 <= 0 || idx1 > size || idx2 > size) {
            cout << "\x1B[0;32mInvalid film numbers\x1b[0m\n";
            return;
        }

        if (films[idx1-1]->getType() == "Film" && films[idx2-1]->getType() == "Film") {
            Film* film1 = dynamic_cast<Film*>(films[idx1-1]);
            Film* film2 = dynamic_cast<Film*>(films[idx2-1]);
            if (*film1 == *film2) {
                cout << "\x1B[0;32mFilms are the same (same year and name)\x1b[0m\n";
            } else {
                cout << "\x1B[0;32mFilms are different\x1b[0m\n";
            }
        } else {
            cout << "\x1B[0;32mOnly films can be compared\x1b[0m\n";
        }
    }

    void createSpecialVersion() {
        if (size < 1) {
            cout << "\x1B[0;32mNo films to create version from\x1b[0m\n";
            return;
        }

        cout << "Enter film number: ";
        int idx;
        cin >> idx;
        cin.ignore();

        if (idx <= 0 || idx > size) {
            cout << "\x1B[0;32mInvalid film number\x1b[0m\n";
            return;
        }

        if (films[idx-1]->getType() == "Film") {
            Film* film = dynamic_cast<Film*>(films[idx-1]);
            if (size >= capacity) {
                resize(capacity * 2);
            }

            cout << "Enter new director name: ";
            string director;
            getline(cin, director);
            films[size++] = new Film(*film, director);
            cout << "\x1B[0;32mDirector's Cut created\x1b[0m\n";
        } else {
            cout << "\x1B[0;32mOnly films can be used to create a Director's Cut\x1b[0m\n";
        }
    }

    int calculateTotalDuration(const Series& series1, const Series& series2) {
        return (series1.episodes * series1.episodeDuration * series1.seasons) +
               (series2.episodes * series2.episodeDuration * series2.seasons);
    }

    void compareTotalDuration() {
        Film::clear();
        cout << "\x1B[5;32mEnter first series number: \x1b[0m";
        size_t idx1;
        cin >> idx1;
        cin.ignore();

        cout << "\x1B[5;32mEnter second series number: \x1b[0m";
        size_t idx2;
        cin >> idx2;
        cin.ignore();

        if (idx1 < 1 || idx1 > size || idx2 < 1 || idx2 > size) {
            cout << "\x1B[0;32mInvalid series numbers\x1b[0m\n";
            return;
        }

        Series* series1 = dynamic_cast<Series*>(films[idx1-1]);
        Series* series2 = dynamic_cast<Series*>(films[idx2-1]);

        if (!series1 || !series2) {
            cout << "\x1B[0;32mBoth items must be series\x1b[0m\n";
            return;
        }

        int totalEpisodes = series1->episodes + series2->episodes;
        int totalDuration = calculateTotalDuration(*series1, *series2);

        cout << "\x1B[0;32mTotal episodes: " << totalEpisodes << "\n";
        cout << "Total duration: " << totalDuration << " minutes (" 
             << (totalDuration / 60) << " hours " << (totalDuration % 60) << " minutes)\x1b[0m\n";
    }

    void resize(size_t newCapacity) {
        MediaContent** newFilms = new MediaContent*[newCapacity];
        for (size_t i = 0; i < size; ++i) {
            newFilms[i] = films[i];
        }
        delete[] films;
        films = newFilms;
        capacity = newCapacity;
    }
};

bool validateDatabaseSize(const FilmDatabase& db) {
    return db.size <= 100 && db.size <= db.capacity;
}

class FileManager {
private:
    string currentFilename;
    bool isFileOpen;

    void writeFilm(ofstream& file, const MediaContent& film) {
        file << film.getName() << "\n"
             << film.getCountry() << "\n"
             << film.getDirector() << "\n"
             << film.getYear() << "\n"
             << film.getType() << "\n";
        film.writeSpecificData(file);
    }

    void readFilm(ifstream& file, MediaContent*& film) {
        string name, country, director, type;
        int year;

        getline(file, name);
        getline(file, country);
        getline(file, director);
        file >> year;
        file.ignore();
        getline(file, type);

        if (type == "Film") {
            film = new Film();
        } else if (type == "Series") {
            film = new Series();
        } else if (type == "MiniSeries") {
            film = new MiniSeries();
        }

        film->setName(name);
        film->setCountry(country);
        film->setDirector(director);
        film->setYear(year);
        film->readSpecificData(file);
    }

public:

    FileManager() : currentFilename(""), isFileOpen(false) {}
    FileManager(const string& filename) : currentFilename(filename), isFileOpen(false) {}

    void setFilename(const string& filename) { currentFilename = filename; }

    bool checkDatabase(const FilmDatabase& db) {
        return validateDatabaseSize(db);
    }

    void saveToFile(const FilmDatabase& database) {
        ofstream file(currentFilename);

        if (!file) {
            cout << "\x1B[0;32mFailed to open file for writing\x1b[0m\n";
            isFileOpen = false;
            return;
        }

        isFileOpen = true;
        size_t size = database.getSize();
        const MediaContent* const* films = database.getFilms();
        file << size << endl;
        for (size_t i = 0; i < size; ++i) {
            writeFilm(file, *films[i]);
        }

        file.close();
        isFileOpen = false;
        Film::clear();
        cout << "\x1B[0;32;40mData saved succesfully\x1b[0m\n";
    }

    void loadFromFile(FilmDatabase& database) {
        ifstream file(currentFilename);

        if (!file) {
            cout << "\x1B[0;32mFailed to open file for reading\x1b[0m\n";
            isFileOpen = false;
            return;
        }

        isFileOpen = true;
        size_t filmCount;
        file >> filmCount;
        file.ignore();

        MediaContent** films = new MediaContent*[filmCount];
        for (size_t i = 0; i < filmCount; ++i) {
            readFilm(file, films[i]);
        }

        database.setFilms(films, filmCount);
        delete[] films;
        file.close();
        isFileOpen = false;
    }

    bool isOpen() const { return isFileOpen; }
};

int main() {

    string filename;
    FileManager fileManager;
    FilmDatabase database;

    int choice;
    do {
        cout << "\n1. Add\n"
                  << "2. Delete film\n"
                  << "3. Print all films\n"
                  << "4. Save\n"
                  << "5. Load\n"
                  << "6. Search\n"
                  << "7. Combine films\n"
                  << "8. Multiply film\n"
                  << "9. Increment film\n"
                  << "10. Compare films\n"
                  << "11. Create Director's Cut\n"
                  << "12. Combine series\n"
                  << "0. Exit\n";
        cout << "Input: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                database.addFilm();
                break;
            case 2:
                database.deleteFilm();
                break;
            case 3:
                database.printFilms();
                break;
            case 4:
                cout << "Input file name: ";
                cin >> filename;
                fileManager.setFilename(filename + ".txt");
                fileManager.saveToFile(database);
                break;
            case 5:
                cout << "Input file name: ";
                cin >> filename;
                fileManager.setFilename(filename + ".txt");
                fileManager.loadFromFile(database);
                break;
            case 6:
                database.searchFilmByActor();
                break;
            case 7:
                database.combineFilms();
                break;
            case 8:
                database.multiplyFilm();
                break;
            case 9:
                database.incrementFilm();
                break;
            case 10:
                database.compareFilms();
                break;
            case 11:
                database.createSpecialVersion();
                break;
            case 12:
                database.compareTotalDuration();
                break;
            case 0:
                cout << "Exit...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 0);

    return 0;
}