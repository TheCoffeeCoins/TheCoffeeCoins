Release Process
====================

## Branch updates

### Before every release candidate

* Update translations see [translation_process.md](https://github.com/thecoffeecoins/thecoffeecoins/blob/master/doc/translation_process.md#synchronising-translations).
* Update manpages, see [gen-manpages.sh](https://github.com/thecoffeecoins/thecoffeecoins/blob/master/contrib/devtools/README.md#gen-manpagessh).
* Update release candidate version in `configure.ac` (`CLIENT_VERSION_RC`).

### Before every major and minor release

* Update [bips.md](bips.md) to account for changes since the last release (don't forget to bump the version number on the first line).
* Update version in `configure.ac` (don't forget to set `CLIENT_VERSION_RC` to `0`).
* Write release notes (see "Write the release notes" below).

### Before every major release

* On both the master branch and the new release branch:
  - update `CLIENT_VERSION_MAJOR` in [`configure.ac`](../configure.ac)
  - update `CLIENT_VERSION_MAJOR`, `PACKAGE_VERSION`, and `PACKAGE_STRING` in [`build_msvc/thecoffeecoins_config.h`](/build_msvc/thecoffeecoins_config.h)
* On the new release branch in [`configure.ac`](../configure.ac) and [`build_msvc/thecoffeecoins_config.h`](/build_msvc/thecoffeecoins_config.h) (see [this commit](https://github.com/thecoffeecoins/thecoffeecoins/commit/742f7dd)):
  - set `CLIENT_VERSION_MINOR` to `0`
  - set `CLIENT_VERSION_BUILD` to `0`
  - set `CLIENT_VERSION_IS_RELEASE` to `true`

#### Before branch-off

* Update hardcoded [seeds](/contrib/seeds/README.md), see [this pull request](https://github.com/thecoffeecoins/thecoffeecoins/pull/7415) for an example.
* Update [`src/chainparams.cpp`](/src/chainparams.cpp) m_assumed_blockchain_size and m_assumed_chain_state_size with the current size plus some overhead (see [this](#how-to-calculate-assumed-blockchain-and-chain-state-size) for information on how to calculate them).
* Update [`src/chainparams.cpp`](/src/chainparams.cpp) chainTxData with statistics about the transaction count and rate. Use the output of the `getchaintxstats` RPC, see
  [this pull request](https://github.com/thecoffeecoins/thecoffeecoins/pull/20263) for an example. Reviewers can verify the results by running `getchaintxstats <window_block_count> <window_final_block_hash>` with the `window_block_count` and `window_final_block_hash` from your output.
* Update `src/chainparams.cpp` nMinimumChainWork and defaultAssumeValid (and the block height comment) with information from the `getblockheader` (and `getblockhash`) RPCs.
  - The selected value must not be orphaned so it may be useful to set the value two blocks back from the tip.
  - Testnet should be set some tens of thousands back from the tip due to reorgs there.
  - This update should be reviewed with a reindex-chainstate with assumevalid=0 to catch any defect
     that causes rejection of blocks in the past history.
- Clear the release notes and move them to the wiki (see "Write the release notes" below).

#### After branch-off (on master)

- Update the version of `contrib/gitian-descriptors/*.yml`.

#### After branch-off (on the major release branch)

- Update the versions.
- Create a pinned meta-issue for testing the release candidate (see [this issue](https://github.com/thecoffeecoins/thecoffeecoins/issues/17079) for an example) and provide a link to it in the release announcements where useful.

#### Before final release

- Merge the release notes from the wiki into the branch.
- Ensure the "Needs release note" label is removed from all relevant pull requests and issues.

#### Tagging a release (candidate)

To tag the version (or release candidate) in git, use the `make-tag.py` script from [thecoffeecoins-maintainer-tools](https://github.com/thecoffeecoins-core/thecoffeecoins-maintainer-tools). From the root of the repository run:

    ../thecoffeecoins-maintainer-tools/make-tag.py v(new version, e.g. 0.20.0)

This will perform a few last-minute consistency checks in the build system files, and if they pass, create a signed tag.

## Building

### First time / New builders

If you're using the automated script (found in [contrib/gitian-build.py](/contrib/gitian-build.py)), then at this point you should run it with the "--setup" command. Otherwise ignore this.

Check out the source code in the following directory hierarchy.

    cd /path/to/your/toplevel/build
    git clone https://github.com/thecoffeecoins-core/gitian.sigs.git
    git clone https://github.com/thecoffeecoins-core/thecoffeecoins-detached-sigs.git
    git clone https://github.com/devrandom/gitian-builder.git
    git clone https://github.com/thecoffeecoins/thecoffeecoins.git

### Write the release notes

Open a draft of the release notes for collaborative editing at https://github.com/thecoffeecoins-core/thecoffeecoins-devwiki/wiki.

For the period during which the notes are being edited on the wiki, the version on the branch should be wiped and replaced with a link to the wiki which should be used for all announcements until `-final`.

Generate the change log. As this is a huge amount of work to do manually, there is the `list-pulls` script to do a pre-sorting step based on github PR metadata. See the [documentation in the README.md](https://github.com/thecoffeecoins-core/thecoffeecoins-maintainer-tools/blob/master/README.md#list-pulls).

Generate list of authors:

    git log --format='- %aN' v(current version, e.g. 0.20.0)..v(new version, e.g. 0.20.1) | sort -fiu

### Setup and perform Gitian builds

If you're using the automated script (found in [contrib/gitian-build.py](/contrib/gitian-build.py)), then at this point you should run it with the "--build" command. Otherwise ignore this.

Setup Gitian descriptors:

    pushd ./thecoffeecoins
    export SIGNER="(your Gitian key, ie bluematt, sipa, etc)"
    export VERSION=(new version, e.g. 0.20.0)
    git fetch
    git checkout v${VERSION}
    popd

Ensure your gitian.sigs are up-to-date if you wish to gverify your builds against other Gitian signatures.

    pushd ./gitian.sigs
    git pull
    popd

Ensure gitian-builder is up-to-date:

    pushd ./gitian-builder
    git pull
    popd

### Fetch and create inputs: (first time, or when dependency versions change)

    pushd ./gitian-builder
    mkdir -p inputs
    wget -O inputs/osslsigncode-2.0.tar.gz https://github.com/mtrojnar/osslsigncode/archive/2.0.tar.gz
    echo '5a60e0a4b3e0b4d655317b2f12a810211c50242138322b16e7e01c6fbb89d92f inputs/osslsigncode-2.0.tar.gz' | sha256sum -c
    popd

Create the macOS SDK tarball, see the [macdeploy instructions](/contrib/macdeploy/README.md#deterministic-macos-dmg-notes) for details, and copy it into the inputs directory.

### Optional: Seed the Gitian sources cache and offline git repositories

NOTE: Gitian is sometimes unable to download files. If you have errors, try the step below.

By default, Gitian will fetch source files as needed. To cache them ahead of time, make sure you have checked out the tag you want to build in thecoffeecoins, then:

    pushd ./gitian-builder
    make -C ../thecoffeecoins/depends download SOURCES_PATH=`pwd`/cache/common
    popd

Only missing files will be fetched, so this is safe to re-run for each build.

NOTE: Offline builds must use the --url flag to ensure Gitian fetches only from local URLs. For example:

    pushd ./gitian-builder
    ./bin/gbuild --url thecoffeecoins=/path/to/thecoffeecoins,signature=/path/to/sigs {rest of arguments}
    popd

The gbuild invocations below <b>DO NOT DO THIS</b> by default.

### Build and sign thecoffeecoins Core for Linux, Windows, and macOS:

    pushd ./gitian-builder
    ./bin/gbuild --num-make 2 --memory 3000 --commit thecoffeecoins=v${VERSION} ../thecoffeecoins/contrib/gitian-descriptors/gitian-linux.yml
    ./bin/gsign --signer "$SIGNER" --release ${VERSION}-linux --destination ../gitian.sigs/ ../thecoffeecoins/contrib/gitian-descriptors/gitian-linux.yml
    mv build/out/thecoffeecoins-*.tar.gz build/out/src/thecoffeecoins-*.tar.gz ../

    ./bin/gbuild --num-make 2 --memory 3000 --commit thecoffeecoins=v${VERSION} ../thecoffeecoins/contrib/gitian-descriptors/gitian-win.yml
    ./bin/gsign --signer "$SIGNER" --release ${VERSION}-win-unsigned --destination ../gitian.sigs/ ../thecoffeecoins/contrib/gitian-descriptors/gitian-win.yml
    mv build/out/thecoffeecoins-*-win-unsigned.tar.gz inputs/thecoffeecoins-win-unsigned.tar.gz
    mv build/out/thecoffeecoins-*.zip build/out/thecoffeecoins-*.exe ../

    ./bin/gbuild --num-make 2 --memory 3000 --commit thecoffeecoins=v${VERSION} ../thecoffeecoins/contrib/gitian-descriptors/gitian-osx.yml
    ./bin/gsign --signer "$SIGNER" --release ${VERSION}-osx-unsigned --destination ../gitian.sigs/ ../thecoffeecoins/contrib/gitian-descriptors/gitian-osx.yml
    mv build/out/thecoffeecoins-*-osx-unsigned.tar.gz inputs/thecoffeecoins-osx-unsigned.tar.gz
    mv build/out/thecoffeecoins-*.tar.gz build/out/thecoffeecoins-*.dmg ../
    popd

Build output expected:

  1. source tarball (`thecoffeecoins-${VERSION}.tar.gz`)
  2. linux 32-bit and 64-bit dist tarballs (`thecoffeecoins-${VERSION}-linux[32|64].tar.gz`)
  3. windows 32-bit and 64-bit unsigned installers and dist zips (`thecoffeecoins-${VERSION}-win[32|64]-setup-unsigned.exe`, `thecoffeecoins-${VERSION}-win[32|64].zip`)
  4. macOS unsigned installer and dist tarball (`thecoffeecoins-${VERSION}-osx-unsigned.dmg`, `thecoffeecoins-${VERSION}-osx64.tar.gz`)
  5. Gitian signatures (in `gitian.sigs/${VERSION}-<linux|{win,osx}-unsigned>/(your Gitian key)/`)

### Verify other gitian builders signatures to your own. (Optional)

Add other gitian builders keys to your gpg keyring, and/or refresh keys: See `../thecoffeecoins/contrib/gitian-keys/README.md`.

Verify the signatures

    pushd ./gitian-builder
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-linux ../thecoffeecoins/contrib/gitian-descriptors/gitian-linux.yml
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-win-unsigned ../thecoffeecoins/contrib/gitian-descriptors/gitian-win.yml
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-osx-unsigned ../thecoffeecoins/contrib/gitian-descriptors/gitian-osx.yml
    popd

### Next steps:

Commit your signature to gitian.sigs:

    pushd gitian.sigs
    git add ${VERSION}-linux/"${SIGNER}"
    git add ${VERSION}-win-unsigned/"${SIGNER}"
    git add ${VERSION}-osx-unsigned/"${SIGNER}"
    git commit -m "Add ${VERSION} unsigned sigs for ${SIGNER}"
    git push  # Assuming you can push to the gitian.sigs tree
    popd

Codesigner only: Create Windows/macOS detached signatures:
- Only one person handles codesigning. Everyone else should skip to the next step.
- Only once the Windows/macOS builds each have 3 matching signatures may they be signed with their respective release keys.

Codesigner only: Sign the macOS binary:

    transfer thecoffeecoins-osx-unsigned.tar.gz to macOS for signing
    tar xf thecoffeecoins-osx-unsigned.tar.gz
    ./detached-sig-create.sh -s "Key ID"
    Enter the keychain password and authorize the signature
    Move signature-osx.tar.gz back to the gitian host

Codesigner only: Sign the windows binaries:

    tar xf thecoffeecoins-win-unsigned.tar.gz
    ./detached-sig-create.sh -key /path/to/codesign.key
    Enter the passphrase for the key when prompted
    signature-win.tar.gz will be created

Codesigner only: Commit the detached codesign payloads:

    cd ~/thecoffeecoins-detached-sigs
    checkout the appropriate branch for this release series
    rm -rf *
    tar xf signature-osx.tar.gz
    tar xf signature-win.tar.gz
    git add -A
    git commit -m "point to ${VERSION}"
    git tag -s v${VERSION} HEAD
    git push the current branch and new tag

Non-codesigners: wait for Windows/macOS detached signatures:

- Once the Windows/macOS builds each have 3 matching signatures, they will be signed with their respective release keys.
- Detached signatures will then be committed to the [thecoffeecoins-detached-sigs](https://github.com/thecoffeecoins-core/thecoffeecoins-detached-sigs) repository, which can be combined with the unsigned apps to create signed binaries.

Create (and optionally verify) the signed macOS binary:

    pushd ./gitian-builder
    ./bin/gbuild -i --commit signature=v${VERSION} ../thecoffeecoins/contrib/gitian-descriptors/gitian-osx-signer.yml
    ./bin/gsign --signer "$SIGNER" --release ${VERSION}-osx-signed --destination ../gitian.sigs/ ../thecoffeecoins/contrib/gitian-descriptors/gitian-osx-signer.yml
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-osx-signed ../thecoffeecoins/contrib/gitian-descriptors/gitian-osx-signer.yml
    mv build/out/thecoffeecoins-osx-signed.dmg ../thecoffeecoins-${VERSION}-osx.dmg
    popd

Create (and optionally verify) the signed Windows binaries:

    pushd ./gitian-builder
    ./bin/gbuild -i --commit signature=v${VERSION} ../thecoffeecoins/contrib/gitian-descriptors/gitian-win-signer.yml
    ./bin/gsign --signer "$SIGNER" --release ${VERSION}-win-signed --destination ../gitian.sigs/ ../thecoffeecoins/contrib/gitian-descriptors/gitian-win-signer.yml
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-win-signed ../thecoffeecoins/contrib/gitian-descriptors/gitian-win-signer.yml
    mv build/out/thecoffeecoins-*win64-setup.exe ../thecoffeecoins-${VERSION}-win64-setup.exe
    popd

Commit your signature for the signed macOS/Windows binaries:

    pushd gitian.sigs
    git add ${VERSION}-osx-signed/"${SIGNER}"
    git add ${VERSION}-win-signed/"${SIGNER}"
    git commit -m "Add ${SIGNER} ${VERSION} signed binaries signatures"
    git push  # Assuming you can push to the gitian.sigs tree
    popd

### After 3 or more people have gitian-built and their results match:

- Create `SHA256SUMS.asc` for the builds, and GPG-sign it:

```bash
sha256sum * > SHA256SUMS
```

The list of files should be:
```
thecoffeecoins-${VERSION}-aarch64-linux-gnu.tar.gz
thecoffeecoins-${VERSION}-arm-linux-gnueabihf.tar.gz
thecoffeecoins-${VERSION}-riscv64-linux-gnu.tar.gz
thecoffeecoins-${VERSION}-x86_64-linux-gnu.tar.gz
thecoffeecoins-${VERSION}-osx64.tar.gz
thecoffeecoins-${VERSION}-osx.dmg
thecoffeecoins-${VERSION}.tar.gz
thecoffeecoins-${VERSION}-win64-setup.exe
thecoffeecoins-${VERSION}-win64.zip
```
The `*-debug*` files generated by the gitian build contain debug symbols
for troubleshooting by developers. It is assumed that anyone that is interested
in debugging can run gitian to generate the files for themselves. To avoid
end-user confusion about which file to pick, as well as save storage
space *do not upload these to the thecoffeecoins.org server, nor put them in the torrent*.

- GPG-sign it, delete the unsigned file:
```
gpg --digest-algo sha256 --clearsign SHA256SUMS # outputs SHA256SUMS.asc
rm SHA256SUMS
```
(the digest algorithm is forced to sha256 to avoid confusion of the `Hash:` header that GPG adds with the SHA256 used for the files)
Note: check that SHA256SUMS itself doesn't end up in SHA256SUMS, which is a spurious/nonsensical entry.

- Upload zips and installers, as well as `SHA256SUMS.asc` from last step, to the thecoffeecoins.org server
  into `/var/www/bin/thecoffeecoins-core-${VERSION}`

- A `.torrent` will appear in the directory after a few minutes. Optionally help seed this torrent. To get the `magnet:` URI use:
```bash
transmission-show -m <torrent file>
```
Insert the magnet URI into the announcement sent to mailing lists. This permits
people without access to `thecoffeecoins.org` to download the binary distribution.
Also put it into the `optional_magnetlink:` slot in the YAML file for
thecoffeecoins.org (see below for thecoffeecoins.org update instructions).

- Update thecoffeecoins.org version

  - First, check to see if the thecoffeecoins.org maintainers have prepared a
    release: https://github.com/thecoffeecoins-dot-org/thecoffeecoins.org/labels/Core

      - If they have, it will have previously failed their CI
        checks because the final release files weren't uploaded.
        Trigger a CI rebuild---if it passes, merge.

  - If they have not prepared a release, follow the thecoffeecoins.org release
    instructions: https://github.com/thecoffeecoins-dot-org/thecoffeecoins.org/blob/master/docs/adding-events-release-notes-and-alerts.md#release-notes

  - After the pull request is merged, the website will automatically show the newest version within 15 minutes, as well
    as update the OS download links.

- Update other repositories and websites for new version

  - thecoffeecoinscore.org blog post

  - thecoffeecoinscore.org maintained versions update:
    [table](https://github.com/thecoffeecoins-core/thecoffeecoinscore.org/commits/master/_includes/posts/maintenance-table.md)

  - thecoffeecoinscore.org RPC documentation update

      - Install [golang](https://golang.org/doc/install)

      - Install the new thecoffeecoins Core release

      - Run thecoffeecoinsd on regtest

      - Clone the [thecoffeecoinscore.org repository](https://github.com/thecoffeecoins-core/thecoffeecoinscore.org)

      - Run: `go run generate.go` while being in `contrib/doc-gen` folder, and with thecoffeecoins-cli in PATH

      - Add the generated files to git

  - Update packaging repo

      - Push the flatpak to flathub, e.g. https://github.com/flathub/org.thecoffeecoinscore.thecoffeecoins-qt/pull/2

      - Push the latest version to master (if applicable), e.g. https://github.com/thecoffeecoins-core/packaging/pull/32

      - Create a new branch for the major release "0.xx" from master (used to build the snap package) and request the
        track (if applicable), e.g. https://forum.snapcraft.io/t/track-request-for-thecoffeecoins-core-snap/10112/7

      - Notify MarcoFalke so that he can start building the snap package

        - https://code.launchpad.net/~thecoffeecoins-core/thecoffeecoins-core-snap/+git/packaging (Click "Import Now" to fetch the branch)
        - https://code.launchpad.net/~thecoffeecoins-core/thecoffeecoins-core-snap/+git/packaging/+ref/0.xx (Click "Create snap package")
        - Name it "thecoffeecoins-core-snap-0.xx"
        - Leave owner and series as-is
        - Select architectures that are compiled via gitian
        - Leave "automatically build when branch changes" unticked
        - Tick "automatically upload to store"
        - Put "thecoffeecoins-core" in the registered store package name field
        - Tick the "edge" box
        - Put "0.xx" in the track field
        - Click "create snap package"
        - Click "Request builds" for every new release on this branch (after updating the snapcraft.yml in the branch to reflect the latest gitian results)
        - Promote release on https://snapcraft.io/thecoffeecoins-core/releases if it passes sanity checks

  - This repo

      - Archive the release notes for the new version to `doc/release-notes/` (branch `master` and branch of the release)

      - Create a [new GitHub release](https://github.com/thecoffeecoins/thecoffeecoins/releases/new) with a link to the archived release notes

- Announce the release:

  - thecoffeecoins-dev and thecoffeecoins-core-dev mailing list

  - thecoffeecoins Core announcements list https://thecoffeecoinscore.org/en/list/announcements/join/

  - Update title of #thecoffeecoins on Freenode IRC

  - Optionally twitter, reddit /r/thecoffeecoins, ... but this will usually sort out itself

  - Celebrate

### Additional information

#### <a name="how-to-calculate-assumed-blockchain-and-chain-state-size"></a>How to calculate `m_assumed_blockchain_size` and `m_assumed_chain_state_size`

Both variables are used as a guideline for how much space the user needs on their drive in total, not just strictly for the blockchain.
Note that all values should be taken from a **fully synced** node and have an overhead of 5-10% added on top of its base value.

To calculate `m_assumed_blockchain_size`:
- For `mainnet` -> Take the size of the data directory, excluding `/regtest` and `/testnet3` directories.
- For `testnet` -> Take the size of the `/testnet3` directory.


To calculate `m_assumed_chain_state_size`:
- For `mainnet` -> Take the size of the `/chainstate` directory.
- For `testnet` -> Take the size of the `/testnet3/chainstate` directory.

Notes:
- When taking the size for `m_assumed_blockchain_size`, there's no need to exclude the `/chainstate` directory since it's a guideline value and an overhead will be added anyway.
- The expected overhead for growth may change over time, so it may not be the same value as last release; pay attention to that when changing the variables.
