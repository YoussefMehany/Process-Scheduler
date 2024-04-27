#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int algo;
int quantum=0;
gchar *filename;
GtkWidget *window;
GtkWidget *entry;
GtkWidget *label_filename;
GtkWidget *listbox_received_strings;

void save_window_as_image(const gchar *filename) {
    GdkWindow *gdk_window = gtk_widget_get_window(window);
    GdkPixbuf *pixbuf = gdk_pixbuf_get_from_window(gdk_window, 0, 0, 
                              gdk_window_get_width(gdk_window), 
                              gdk_window_get_height(gdk_window));
    if (pixbuf != NULL) {
        gdk_pixbuf_save(pixbuf, filename, "png", NULL, NULL);
        g_object_unref(pixbuf);
        g_print("Window saved as %s\n", filename);
    } else {
        g_print("Failed to capture window\n");
    }
}

void clear_window() {
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(window));
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
}

void add_received_string(const gchar *received_string) {
    GtkWidget *label = gtk_label_new(received_string);
    gtk_container_add(GTK_CONTAINER(listbox_received_strings), label);
    gtk_label_set_xalign(GTK_LABEL(label), 0); // Center align the label
    gtk_widget_show(label);
}

void add_labels() {
    GtkWidget *vbox; // Vertical box container
    GtkWidget *button_rr;

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create a list box for received strings
    listbox_received_strings = gtk_list_box_new();
    gtk_box_pack_start(GTK_BOX(vbox), listbox_received_strings, FALSE, FALSE, 10);

    //add_received_string("ID    arrivalTime    runtime    priority    waitingtime    remainingTime     state");
    GdkRGBA entry_bg_color;
    gdk_rgba_parse(&entry_bg_color, "#2f2f2f");
    gtk_widget_override_background_color(listbox_received_strings, GTK_STATE_FLAG_NORMAL, &entry_bg_color);
    // Show the list box
    gtk_widget_show_all(window);
}

void run_generator(){
    pid_t pid = fork();
    int guipid = getpid();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char* args[5];
        args[0] = "./process_generator.out";
        args[1] = (char *)malloc(12);
        sprintf(args[1], "%d", algo);
        args[2] = (char *)malloc(15);
        sprintf(args[2], "%d", guipid);
        args[3] = (char *)malloc(15);
        sprintf(args[3], "%d", quantum);
        args[4] = NULL;
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
}

void on_entry_activate(GtkWidget *entry, gpointer data) {
    filename = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
    g_print("Entered filename: %s\n", filename);
    clear_window(GTK_WIDGET(data));
    add_labels(GTK_WIDGET(data));
    kill(getpid(),SIGUSR1); 
}

void add_label_and_entry() {
    GtkWidget *vbox; // Vertical box container
    GtkWidget *hbox_buttons; // Horizontal box container for buttons

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    label_filename = gtk_label_new("Enter name of output file:");
    gtk_widget_set_name(label_filename, "custom-label");
    gtk_label_set_xalign(GTK_LABEL(label_filename), 0.5);
    gtk_box_pack_start(GTK_BOX(vbox), label_filename, FALSE, FALSE, 20);

    hbox_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_set_homogeneous(GTK_BOX(hbox_buttons), TRUE); 
    gtk_box_pack_start(GTK_BOX(vbox), hbox_buttons, FALSE, FALSE, 0);

    // Create and add the text entry field
    entry = gtk_entry_new();
    gtk_widget_set_name(entry, "custom-entry");
    gtk_widget_set_size_request(entry, 400, 50); // Set width to 200 pixels and height to 30 pixels
    gtk_box_pack_start(GTK_BOX(hbox_buttons), entry, FALSE, FALSE, 0);
    g_signal_connect(entry, "activate", G_CALLBACK(on_entry_activate), window);

    // Apply custom styles for the entry field
    gtk_widget_override_color(label_filename, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){1, 1, 1, 1});
    gtk_widget_override_color(entry, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){1, 1, 1, 1}); // White text color
    PangoFontDescription *font_desc = pango_font_description_from_string("Arial 15");
    gtk_widget_override_font(label_filename, font_desc);
    pango_font_description_free(font_desc);

    // Set the background color of the entry widget
    GdkRGBA entry_bg_color;
    gdk_rgba_parse(&entry_bg_color, "#2f2f2f");
    gtk_widget_override_background_color(entry, GTK_STATE_FLAG_NORMAL, &entry_bg_color);


    // Show the label and entry field
    gtk_widget_show_all(window);
}

void on_entry_activate_quantum(GtkWidget *entry2, gpointer user_data) {
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry2));
    int quantum = atoi(text); // Convert text to integer
    g_print("Entered value: %d\n", quantum);
    clear_window(window);
    //run_generator();
    add_label_and_entry(); 
}

void on_entry_changed_quantum(GtkWidget *entry, gpointer user_data) {
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    if (strlen(text) > 2) {
        gchar truncated_text[3]; // Allow only two digits
        strncpy(truncated_text, text, 2); // Copy the first two digits
        truncated_text[2] = '\0'; // Null-terminate the string
        gtk_entry_set_text(GTK_ENTRY(entry), truncated_text); // Update the entry text
    }
    for (int i = 0; i < strlen(text); i++) {
        if (!g_ascii_isdigit(text[i])) {
            gtk_entry_set_text(GTK_ENTRY(entry), ""); // Clear the entry if non-digit characters are entered
            break;
        }
    }
}

void add_number_input_quantum() {
    GtkWidget *vbox; // Vertical box container
    GtkWidget *hbox_buttons; // Horizontal box container for buttons

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);


    GtkWidget *label2 = gtk_label_new("Enter Quantum(MAX 2 numbers)");
    gtk_widget_set_name(label2, "custom-label-quantum");
    gtk_label_set_xalign(GTK_LABEL(label2), 0.5);
    gtk_box_pack_start(GTK_BOX(vbox), label2, FALSE, FALSE, 20);

    hbox_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_set_homogeneous(GTK_BOX(hbox_buttons), TRUE); 
    gtk_box_pack_start(GTK_BOX(vbox), hbox_buttons, FALSE, FALSE, 0);


    GtkWidget *entry2 = gtk_entry_new();
    gtk_widget_set_name(entry2, "custom-entry-quantum");
    gtk_widget_set_size_request(entry2, 10, 50); 
    gtk_entry_set_input_purpose(GTK_ENTRY(entry2), GTK_INPUT_PURPOSE_NUMBER);
    gtk_box_pack_start(GTK_BOX(hbox_buttons), entry2, FALSE, FALSE, 0);
    g_signal_connect(entry2, "changed", G_CALLBACK(on_entry_changed_quantum), NULL);
    g_signal_connect(entry2, "activate", G_CALLBACK(on_entry_activate_quantum), NULL);

    //styles
    gtk_widget_override_color(label2, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){1, 1, 1, 1});
    gtk_widget_override_color(entry2, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){1, 1, 1, 1}); // White text color
    PangoFontDescription *font_desc = pango_font_description_from_string("Arial 15");
    gtk_widget_override_font(label2, font_desc);
    pango_font_description_free(font_desc);

    GdkRGBA entry_bg_color;
    gdk_rgba_parse(&entry_bg_color, "#2f2f2f");
    gtk_widget_override_background_color(entry2, GTK_STATE_FLAG_NORMAL, &entry_bg_color);

    gtk_widget_show_all(window);
}

void on_button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    g_print("Button %s clicked\n", button_label);
    if (strcmp(button_label, "Round Robin") == 0) {
        algo = 1;
    } else if (strcmp(button_label, "Highest Priority First") == 0) {
        algo = 3;
    } else if (strcmp(button_label, "Shortest Remaining Time Next") == 0) {
        algo = 2;
    }
    g_print("Button %d clicked\n", algo);
    clear_window(GTK_WIDGET(data));
    if(algo == 1){
        add_number_input_quantum();
    }
    else{
    //run_generator();
    add_label_and_entry(); 
    }
}

gboolean save_image_delayed(gpointer user_data) {
    save_window_as_image("screenshot.png");
    return G_SOURCE_REMOVE;  // Remove the timeout after it's executed once
}

void read_file_and_process_lines() {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        g_print("Failed to open file\n");
        return;
    }
    char line[256]; // Adjust the size as needed
    while (fgets(line, sizeof(line), file) != NULL) {
        // Process each line here
        add_received_string(line);
        //g_print("Read line: %s", line);
    }
    fclose(file);
    g_timeout_add(500, save_image_delayed, NULL); 
    //save_window_as_image("screenshot.png");
}

gboolean signal_handler1(gint signum) {
    // Handle the signal here
    read_file_and_process_lines();
    return TRUE; // Return TRUE to keep the signal handler connected
}

int main(int argc, char *argv[]) {
    GtkWidget *vbox; // Vertical box container
    GtkWidget *hbox_buttons; // Horizontal box container for buttons
    GtkWidget *label1, *label2;
    GtkWidget *button_rr, *button_hpf, *button_srtn;

    gtk_init(&argc,&argv);

    g_unix_signal_add(SIGUSR1, (GSourceFunc)signal_handler1, NULL);

    // Create a new window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Process Scheduler");
    gtk_widget_set_size_request(window, 1000, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box container
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create and add the "Welcome" label
    label1 = gtk_label_new("Welcome to our Process Scheduler");
    gtk_widget_set_name(label1, "custom-label");
    gtk_label_set_xalign(GTK_LABEL(label1), 0.5); // Center align the label
    gtk_box_pack_start(GTK_BOX(vbox), label1, FALSE, FALSE, 70); // Set margin top to 20px

    // Create and add the "Select Algorithm" label
    label2 = gtk_label_new("Please select your algorithm:");
    gtk_widget_set_name(label2, "custom-label");
    gtk_label_set_xalign(GTK_LABEL(label2), 0.5); // Center align the label
    gtk_box_pack_start(GTK_BOX(vbox), label2, FALSE, FALSE, 0);

    // Create a horizontal box container for buttons
    hbox_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_set_homogeneous(GTK_BOX(hbox_buttons), TRUE); 
    gtk_box_pack_start(GTK_BOX(vbox), hbox_buttons, FALSE, FALSE, 30);

    // Create and add the "RR" button
    button_rr = gtk_button_new_with_label("Round Robin");
    gtk_widget_set_name(button_rr, "custom-button");
    gtk_widget_set_size_request(button_rr, 100, 50); // Set size to 100x50
    gtk_box_pack_start(GTK_BOX(hbox_buttons), button_rr, FALSE, FALSE, 0); // Set expand and fill to FALSE
    g_signal_connect(button_rr, "clicked", G_CALLBACK(on_button_clicked), window);

    // Create and add the "HPF" button
    button_hpf = gtk_button_new_with_label("Highest Priority First");
    gtk_widget_set_name(button_hpf, "custom-button");
    gtk_widget_set_size_request(button_hpf, 100, 50); // Set size to 100x50
    gtk_box_pack_start(GTK_BOX(hbox_buttons), button_hpf, FALSE, FALSE, 0); // Set expand and fill to FALSE
    g_signal_connect(button_hpf, "clicked", G_CALLBACK(on_button_clicked), window);

    // Create and add the "SRTN" button
    button_srtn = gtk_button_new_with_label("Shortest Remaining Time Next");
    gtk_widget_set_name(button_srtn, "custom-button");
    gtk_widget_set_size_request(button_srtn, 100, 50); // Set size to 100x50
    gtk_box_pack_start(GTK_BOX(hbox_buttons), button_srtn, FALSE, FALSE, 0); // Set expand and fill to FALSE
    g_signal_connect(button_srtn, "clicked", G_CALLBACK(on_button_clicked), window);




    // Apply custom styles
    GdkRGBA window_color;
    gdk_rgba_parse(&window_color, "#302c2c");
    gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &window_color);
    // Set font size and color for labels
    PangoFontDescription *font_desc = pango_font_description_from_string("Arial 30");
    gtk_widget_override_font(label1, font_desc);
    pango_font_description_free(font_desc);

    font_desc = pango_font_description_from_string("Arial 16");
    gtk_widget_override_font(label2, font_desc);
    pango_font_description_free(font_desc);

    // Set text color for labels
    gtk_widget_override_color(label1, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){1, 1, 1, 1}); // Blue color
    gtk_widget_override_color(label2, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){1, 1, 1, 1}); // Blue color

    // Set button styles

// Set the background color of the button widget
    GdkRGBA button_bg_color;
    gdk_rgba_parse(&button_bg_color, "#373737"); // Change "blue" to your desired color

    gtk_widget_override_color(button_rr, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){1, 0, 0, 1}); // White text color

    gtk_widget_override_background_color(button_rr, GTK_STATE_FLAG_NORMAL, &button_bg_color);

    gtk_widget_override_color(button_hpf, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){0, 1, 0, 1}); // White text color

    gtk_widget_override_background_color(button_hpf, GTK_STATE_FLAG_NORMAL, &button_bg_color);

    gtk_widget_override_color(button_srtn, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){0, 0, 1, 1}); // White text color

    gtk_widget_override_background_color(button_srtn, GTK_STATE_FLAG_NORMAL, &button_bg_color);



    // Show the window and all its child widgets
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    

    return 0;
}


//gcc -o GUI GUI.c $(pkg-config --cflags --libs gtk+-3.0)