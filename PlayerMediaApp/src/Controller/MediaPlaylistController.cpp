#include "../../include/Controller/MediaPlaylistController.h"
#include "../../include/utils/PlaylistHandlerException.h"
MediaPlaylistController::MediaPlaylistController(std::shared_ptr<Playlist> p, std::shared_ptr<ViewBase> v) : playlist(p), playlistHandlerView(v){}

void MediaPlaylistController::addMediaFile(std::shared_ptr<MediaFile> mediaFile) {
    try {
        std::string input;

        // Vòng lặp yêu cầu nhập lại nếu đầu vào không hợp lệ
        while (true) {
            std::cout << "Do you want to add media file '" << mediaFile->getName() << "' to the playlist? (Y/N): ";
            std::cin >> input;

            try {
                // Kiểm tra đầu vào hợp lệ (chỉ cho phép 'Y' hoặc 'N')
                validateInputYorN(input); // Nếu sai, exception sẽ được ném ra
                break; // Nếu đầu vào hợp lệ, thoát khỏi vòng lặp
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }

        if (input == "Y" || input == "y") {
            // Kiểm tra tên file hợp lệ
            validateMediaFileName(mediaFile->getName());

            // Kiểm tra file có tồn tại trong playlist chưa
            ensureFileDoesNotExistInPlaylist(playlist, mediaFile);

            // Nếu tất cả hợp lệ, thêm media file vào playlist
            playlist->addMediaFile(mediaFile);
            std::cout << "Media file '" << mediaFile->getName() << "' added successfully." << std::endl;
        } else {
            std::cout << "File addition cancelled." << std::endl;
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void MediaPlaylistController::deleteMediaFile() {
    std::cout << "Enter the name of the file you want to delete: ";
    std::string fileName;
    std::cin >> fileName;

    try {
        // Kiểm tra tên file hợp lệ
        validateMediaFileName(fileName);

        // Kiểm tra file có tồn tại trong playlist không
        ensureMediaFileExists(playlist, fileName);

        // Xóa file nếu tất cả hợp lệ
        playlist->deleteMediaFile(fileName);
        std::cout << "Media file '" << fileName << "' deleted successfully." << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
void MediaPlaylistController::displayAllMediaFiles() {
    try {
        // Kiểm tra playlist có trống không
        ensurePlaylistNotEmpty(playlist);

        // Hiển thị tất cả các file nếu playlist không trống
        playlistHandlerView->displayAllMediaFileInPlaylist(playlist);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}