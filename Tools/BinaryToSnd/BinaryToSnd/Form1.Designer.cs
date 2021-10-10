
namespace BinaryToCpp
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.FileListView = new System.Windows.Forms.ListView();
            this.OutputFileTextBox = new System.Windows.Forms.TextBox();
            this.SelectFilesButton = new System.Windows.Forms.Button();
            this.SelectOutputButton = new System.Windows.Forms.Button();
            this.ProcessFiles = new System.Windows.Forms.Button();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.SuspendLayout();
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            this.openFileDialog1.Filter = "raw files | *.raw";
            this.openFileDialog1.Multiselect = true;
            // 
            // FileListView
            // 
            this.FileListView.HideSelection = false;
            this.FileListView.Location = new System.Drawing.Point(12, 52);
            this.FileListView.Name = "FileListView";
            this.FileListView.Size = new System.Drawing.Size(546, 347);
            this.FileListView.TabIndex = 0;
            this.FileListView.UseCompatibleStateImageBehavior = false;
            this.FileListView.View = System.Windows.Forms.View.List;
            // 
            // OutputFileTextBox
            // 
            this.OutputFileTextBox.Location = new System.Drawing.Point(12, 461);
            this.OutputFileTextBox.Name = "OutputFileTextBox";
            this.OutputFileTextBox.Size = new System.Drawing.Size(546, 22);
            this.OutputFileTextBox.TabIndex = 1;
            // 
            // SelectFilesButton
            // 
            this.SelectFilesButton.Location = new System.Drawing.Point(12, 23);
            this.SelectFilesButton.Name = "SelectFilesButton";
            this.SelectFilesButton.Size = new System.Drawing.Size(113, 23);
            this.SelectFilesButton.TabIndex = 2;
            this.SelectFilesButton.Text = "Select Files";
            this.SelectFilesButton.UseVisualStyleBackColor = true;
            this.SelectFilesButton.Click += new System.EventHandler(this.SelectFilesButton_Click);
            // 
            // SelectOutputButton
            // 
            this.SelectOutputButton.Location = new System.Drawing.Point(12, 432);
            this.SelectOutputButton.Name = "SelectOutputButton";
            this.SelectOutputButton.Size = new System.Drawing.Size(126, 23);
            this.SelectOutputButton.TabIndex = 3;
            this.SelectOutputButton.Text = "Select Output File";
            this.SelectOutputButton.UseVisualStyleBackColor = true;
            this.SelectOutputButton.Click += new System.EventHandler(this.SelectOutputButton_Click);
            // 
            // ProcessFiles
            // 
            this.ProcessFiles.Location = new System.Drawing.Point(441, 515);
            this.ProcessFiles.Name = "ProcessFiles";
            this.ProcessFiles.Size = new System.Drawing.Size(117, 23);
            this.ProcessFiles.TabIndex = 4;
            this.ProcessFiles.Text = "Generate";
            this.ProcessFiles.UseVisualStyleBackColor = true;
            this.ProcessFiles.Click += new System.EventHandler(this.ProcessFiles_Click);
            // 
            // saveFileDialog1
            // 
            this.saveFileDialog1.DefaultExt = "cpp";
            this.saveFileDialog1.Filter = "CPP Files | *.cpp";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(570, 550);
            this.Controls.Add(this.ProcessFiles);
            this.Controls.Add(this.SelectOutputButton);
            this.Controls.Add(this.SelectFilesButton);
            this.Controls.Add(this.OutputFileTextBox);
            this.Controls.Add(this.FileListView);
            this.Name = "Form1";
            this.Text = "BinaryToSnd";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.ListView FileListView;
        private System.Windows.Forms.TextBox OutputFileTextBox;
        private System.Windows.Forms.Button SelectFilesButton;
        private System.Windows.Forms.Button SelectOutputButton;
        private System.Windows.Forms.Button ProcessFiles;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
    }
}

