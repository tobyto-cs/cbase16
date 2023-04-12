#include <utils/updater.hpp>

int handle_git_err(const int& err, int line = -1) {
  string log, err_msg;
  if (err != 0) err_msg = git_error_last()->message;
  if (line != -1) log = fmt::format("UNHANDLED GIT EXCEPTION, LINE: {}, ERR {}: {}\n", err, line, err_msg);
  else log = fmt::format("UNHANDLED GIT EXCEPTION, ERR {}: {}\n", err, err_msg);
  switch(err) {
    case 0:
      return 0;
    case GIT_ENOTFOUND:
      return -3;
    default:
      cout << log;
      return err;
  }
}

int fetchhead_ref_cb(const char *name, const char *url,
    const git_oid *oid, unsigned int is_merge, void *payload)
{
  (void)name;
  (void)url;

  if (is_merge)
  {
    git_oid_cpy((git_oid *)payload, oid);
  }
  return 0;
}

// REQUIRES initalization
int pull_updates(git_repository *repo)
{
  git_remote *remote;

  // Lookup Remote origin
  handle_git_err(git_remote_lookup(&remote, repo, "origin"));
  fmt::print("  Remote name is: {}\n", git_remote_name(remote));

  handle_git_err(git_remote_fetch(remote, NULL, NULL, NULL));

  // Get ID to merge into
  git_oid id_to_merge;
  git_repository_fetchhead_foreach(repo, fetchhead_ref_cb, &id_to_merge);
  fmt::print("  ID of merge is: {}\n", git_oid_tostr_s(&id_to_merge));


  // Get the commit from merge ID
  git_annotated_commit *merge[1];
  handle_git_err(git_annotated_commit_from_fetchhead(
        &merge[0],
        repo,
        git_remote_name(remote),
        git_remote_url(remote),
        &id_to_merge
        ));

  fmt::print("  Annotated Commit is: {}\n", git_annotated_commit_ref(merge[0]));

  // Merge analysis
  git_merge_analysis_t anout;
  git_merge_preference_t pout;
  handle_git_err(git_merge_analysis(&anout, &pout, repo, (const git_annotated_commit **)merge, 1), __LINE__);

  if (anout & GIT_MERGE_ANALYSIS_UP_TO_DATE) {
    fmt::print("  up to date");
  } else if (anout & GIT_MERGE_ANALYSIS_FASTFORWARD) {
    fmt::print("  fast-fowarding...");
    /* git_reference *ref; */
    // retrieve reference to head
    /* handle_git_err(git_repository_head(&ref, repo)); */
    /* git_reference_set_target(&newref, ref, &branchOid_to_merge, "pull: Fast-foward"); */

    git_reset_from_annotated(repo, merge[0], GIT_RESET_HARD, NULL);

    /* git_reference_free(ref); */
    /* git_reference_free(newref); */

  } else {
    fmt::print("  unable to pull branch");
  }
  git_annotated_commit_free(*merge);
  git_repository_state_cleanup(repo);
  git_remote_free(remote);

  return 0;
}


namespace cbase {
  bool exist_create_dir(const fs::path& fp)
  {
    if (fs::exists(fp) && fs::is_directory(fp)) return true;

    std::error_code ec;
    fs::create_directories(fp, ec);
    // TODO: Error code handling
    return true;
  }

  bool handle_curl_res(const CURLcode& res, const string& errstr)
  {
    // TODO: Custom exceptions for this
    switch(res)
    {
      case (CURLE_OK):
        return true;
      default:
        throw std::runtime_error("Unhandled CURL Exception:"+errstr);
        return false;
    }
  };

  bool download_from_url(const string& url, const fs::path& out)
  {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    char errstr[CURL_ERROR_SIZE];
    curl = curl_easy_init();
    if (curl) {
      fp = fopen(out.string().c_str(), "wb");
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
      curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errstr);
      res = curl_easy_perform(curl);
      curl_easy_cleanup(curl);
      fclose(fp);
    }
    return handle_curl_res(res, errstr);
  }



  // Downloads git repos from file
  bool list_git(const fs::path& dir, const bool& debug)
  {
    git_libgit2_init();
    fs::path list = dir / "list.yaml";
    if (!fs::exists(list)) return false;
    if (debug) fmt::print("Cloning repos from {}\n", list.string());

    ifstream file;
    file.exceptions(std::ios_base::badbit);
    file.open(list);
    string list_buf((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(list_buf));

    string name, git_url;
    for (ryml::NodeRef n : tree.rootref().children())
    {
      c4::from_chars(n.key(), &name);
      c4::from_chars(n.val(), &git_url);
      fmt::print("Updating git repo {}\n", name);

      git_repository *repo = NULL;

      if (!fs::exists(dir / name)) {
        fmt::print("  Cloning into repo...");
        fs::create_directories(dir / name);
        handle_git_err(git_clone(&repo, git_url.c_str(), (dir / name).c_str(), NULL));
      } else {
        if (handle_git_err(git_repository_open(&repo, (dir / name).c_str())) == GIT_ENOTFOUND) {
          fmt::print("  Invalid repo, skipping...");
        } else pull_updates(repo);
      }

      git_repository_free(repo);
      fmt::print("\n");
    }
    git_libgit2_shutdown();
    return true;
  }

  bool do_updates(const bool& debug)
  {
    if (debug) fmt::print("Inside do_updates");

    // Validate and setup config directory
    validate_config();

    // retreive list.yaml from git repo
    if (!fs::exists(CBASE_TEMPLATES_LIST))
      download_from_url(CBASE_TEMPLATES_URL, CBASE_TEMPLATES_LIST);
    if (!fs::exists(CBASE_SCHEMES_LIST))
      download_from_url(CBASE_SCHEMES_URL, CBASE_SCHEMES_LIST);

    update_templates();
    update_schemes();

    return true;
  }

  inline string bts(const bool& b) {
    return b ? "✓" : "X";
  }

  int validate_config(const bool& debug)
  {
    if (debug) fmt::print("Validating configuration...");

    // Check the status of required files and directories
    check_updates(debug);

    // Initalize filestructure
    exist_create_dir(CBASE_CONFIG_DIR);
    exist_create_dir(CBASE_SCHEMES_DIR);
    exist_create_dir(CBASE_TEMPLATES_DIR);


    return true;
  }

  bool check_updates(const bool& debug)
  {
    fmt::print(stdout, "Config File\n");
    fmt::print(stdout, "\tconfig dir:   {}\n", bts(fs::exists(CBASE_CONFIG_DIR)));
    if (debug) fmt::print(stdout, "\t\t{}\n", CBASE_CONFIG_DIR.string());
    fmt::print(stdout, "\tconfig.yaml:  {}\n", bts(fs::exists(CBASE_CONFIG_DIR / "config.yaml")));
    if (debug) fmt::print(stdout, "\t\t{}\n", (CBASE_CONFIG_DIR / "config.yaml").string());
    fmt::print(stdout, "Templates\n");
    fmt::print(stdout, "\ttemplate dir: {}\n", bts(fs::exists(CBASE_TEMPLATES_DIR)));
    if (debug) fmt::print(stdout, "\t\t{}\n", CBASE_TEMPLATES_DIR.string());
    fmt::print(stdout, "\tlist.yaml:    {}\n", bts(fs::exists(CBASE_TEMPLATES_LIST)));
    if (debug) fmt::print(stdout, "\t\t{}\n", CBASE_TEMPLATES_LIST.string());
    fmt::print(stdout, "Schemes\n");
    fmt::print(stdout, "\tscheme dir:   {}\n", bts(fs::exists(CBASE_SCHEMES_DIR)));
    if (debug) fmt::print(stdout, "\t\t{}\n", CBASE_SCHEMES_DIR.string());
    fmt::print(stdout, "\tlist.yaml:    {}\n", bts(fs::exists(CBASE_SCHEMES_LIST)));
    if (debug) fmt::print(stdout, "\t\t{}\n", CBASE_SCHEMES_LIST.string());

    return true;
  }

}
